#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::cancellableCollar (const Data::CashFlow &rCap, double dFloorRate,
                        InterestRateModel &rModel)
{
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
  double dCapFactor = (1. + rCap.rate * rCap.period);
  double dFloorFactor = (1. + dFloorRate * rCap.period);
  // value of next caplet
  Slice uCap = max (1. - uDiscount * dCapFactor, 0.);
  // value of next flooret
  Slice uFloor = max (uDiscount * dFloorFactor - 1., 0.);
  Slice uOption = (uCap - uFloor);

  while (iTime > 0)
    {
      // uOption is the value to continue
      // we multiply on notional at the end
      uOption = max (uOption, 0.);
      iTime--;
      uOption.rollback (iTime);
      uDiscount
          = rModel.discount (iTime, rModel.eventTimes ()[iTime] + rCap.period);
      uCap = max (1. - uDiscount * dCapFactor, 0.);
      uFloor = max (uDiscount * dFloorFactor - 1., 0.);
      uOption += (uCap - uFloor);
    }

  uOption *= rCap.notional;

  return interpolate (uOption);
}
