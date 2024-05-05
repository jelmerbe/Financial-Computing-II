#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::fxCrossCurrencyCap (const Data::CashFlow &rCap, AssetModel &rModel)
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
  double dMaturity = rModel.eventTimes ()[iTime] + rCap.period;
  Slice uDiscount = rModel.discount (iTime, dMaturity);
  Slice uForward = rModel.forward (iTime, dMaturity);
  double dSpread = rCap.rate * rCap.period;
  // value of the next payment
  Slice uOption
      = max (1. - uDiscount * dSpread - rModel.spot (iTime) / uForward, 0.);

  while (iTime > 0)
    {
      // uOption is the value of the future payments
      // we multiply on the notional at the end
      iTime--;
      uOption.rollback (iTime);
      dMaturity = rModel.eventTimes ()[iTime] + rCap.period;
      uDiscount = rModel.discount (iTime, dMaturity);
      uForward = rModel.forward (iTime, dMaturity);
      uOption += max (
          1. - uDiscount * dSpread - rModel.spot (iTime) / uForward, 0.);
    }

  uOption *= rCap.notional;

  return interpolate (uOption);
}
