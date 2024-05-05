#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::Slice
rate (unsigned iTime, double dPeriod, const InterestRateModel &rModel)
{
  PRECONDITION (iTime < rModel.eventTimes ().size ());
  PRECONDITION (dPeriod > cfl::EPS);

  double dTime = rModel.eventTimes ()[iTime] + dPeriod;
  cfl::Slice uDiscount = rModel.discount (iTime, dTime);

  return (1. / uDiscount - 1.) / dPeriod;
}

cfl::MultiFunction
prb::futuresOnRate (double dRatePeriod, unsigned iFuturesTimes,
                    double dMaturity, InterestRateModel &rModel)
{
  PRECONDITION (rModel.initialTime () < dMaturity);
  PRECONDITION (iFuturesTimes > 0);

  double dPeriod = (dMaturity - rModel.initialTime ()) / (iFuturesTimes);
  std::vector<double> uEventTimes (iFuturesTimes + 1);
  uEventTimes.front () = rModel.initialTime ();
  std::transform (uEventTimes.begin (), uEventTimes.end () - 1,
                  uEventTimes.begin () + 1,
                  [dPeriod] (double dX) { return dX + dPeriod; });
  ASSERT (std::abs (uEventTimes.back () - dMaturity) < cfl::EPS);
  rModel.assignEventTimes (uEventTimes);

  int iTime = rModel.eventTimes ().size () - 1;
  Slice uFutures = 1. - rate (iTime, dRatePeriod, rModel);
  while (iTime > 0)
    {
      iTime--;
      uFutures.rollback (iTime);
      uFutures
          /= rModel.discount (iTime, rModel.eventTimes ()[iTime] + dPeriod);
    }

  return interpolate (uFutures);
}
