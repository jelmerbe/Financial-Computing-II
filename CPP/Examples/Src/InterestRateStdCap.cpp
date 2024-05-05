#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::cap (const Data::CashFlow &rCap, InterestRateModel &rModel)
{
  // event times: initial time + payment times except the last one
  std::vector<double> uEventTimes (rCap.numberOfPayments);
  uEventTimes.front () = rModel.initialTime ();
  std::transform (uEventTimes.begin (), uEventTimes.end () - 1,
                  uEventTimes.begin () + 1,
                  [&rCap] (double dX) { return dX + rCap.period; });
  rModel.assignEventTimes (uEventTimes);

  // last minus one payment time
  int iTime = uEventTimes.size () - 1;
  Slice uDiscount
      = rModel.discount (iTime, rModel.eventTimes ()[iTime] + rCap.period);
  double dCapFactor = 1. + rCap.rate * rCap.period;
  // value of the next payment
  Slice uOption = max (1. - uDiscount * dCapFactor, 0.);

  while (iTime > 0)
    {
      // uOption is the value of future caplets
      // we multiply on notional at the end
      iTime--;
      uOption.rollback (iTime);
      uDiscount
          = rModel.discount (iTime, rModel.eventTimes ()[iTime] + rCap.period);
      uOption += max (1. - uDiscount * dCapFactor, 0.);
    }

  uOption *= rCap.notional;

  return interpolate (uOption);
}
