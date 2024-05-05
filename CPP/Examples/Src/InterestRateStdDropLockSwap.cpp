#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace std;
using namespace cfl;
using namespace NExamples;

cfl::Slice
swapRate (unsigned iTime, double dPeriod, unsigned iPeriods,
          const InterestRateModel &rModel)
{
  cfl::Slice uFixed = rModel.cash (iTime, 0.);
  double dTime = rModel.eventTimes ()[iTime];
  for (unsigned iI = 0; iI < iPeriods; iI++)
    {
      dTime += dPeriod;
      uFixed += rModel.discount (iTime, dTime);
    }
  uFixed *= dPeriod;
  cfl::Slice uFloat = 1. - rModel.discount (iTime, dTime);
  cfl::Slice uRate = (uFloat / uFixed);

  return uRate;
}

cfl::MultiFunction
prb::dropLockSwap (const Data::Swap &rSwap, double dLowerRate,
                   double dUpperRate, InterestRateModel &rModel)
{
  PRECONDITION (dLowerRate < dUpperRate);

  // event times: initial time + payment times except the last
  std::vector<double> uEventTimes (rSwap.numberOfPayments);
  uEventTimes.front () = rModel.initialTime ();
  std::transform (uEventTimes.begin (), uEventTimes.end () - 1,
                  uEventTimes.begin () + 1,
                  [&rSwap] (double dX) { return dX + rSwap.period; });
  rModel.assignEventTimes (uEventTimes);

  // last minus one payment time
  int iTime = rModel.eventTimes ().size () - 1;
  Data::Swap uSwapOnePeriod (rSwap);
  uSwapOnePeriod.numberOfPayments = 1;
  Slice uOption = NExamples::swap (iTime, uSwapOnePeriod, rModel);
  Data::Swap uSwap (rSwap);

  while (iTime > 0)
    {
      // uOption is the value to continue: the value of future payments if no
      // resets in the past and today
      Slice uSwapRate
          = swapRate (iTime, rSwap.period, rSwap.numberOfPayments, rModel);
      uSwap.numberOfPayments = rModel.eventTimes ().size () - iTime;
      uSwap.rate = dLowerRate;
      uOption += (NExamples::swap (iTime, uSwap, rModel) - uOption)
                 * indicator (dLowerRate, uSwapRate);
      uSwap.rate = dUpperRate;
      uOption += (NExamples::swap (iTime, uSwap, rModel) - uOption)
                 * indicator (uSwapRate, dUpperRate);
      iTime--;
      uOption.rollback (iTime);
      uOption += NExamples::swap (iTime, uSwapOnePeriod, rModel);
    }

  return interpolate (uOption);
}
