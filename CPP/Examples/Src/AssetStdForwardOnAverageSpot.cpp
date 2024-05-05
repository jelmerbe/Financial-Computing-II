#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::forwardOnAverageSpot (const std::vector<double> &rAverTimes,
                           cfl::AssetModel &rModel)
{
  PRECONDITION (rAverTimes.front () > rModel.initialTime ());
  PRECONDITION (std::is_sorted (rAverTimes.begin (), rAverTimes.end (),
                                std::less_equal<double> ()));

  std::vector<double> uEventTimes (rAverTimes.size () + 1);
  uEventTimes.front () = rModel.initialTime ();
  copy (rAverTimes.begin (), rAverTimes.end (), uEventTimes.begin () + 1);
  rModel.assignEventTimes (uEventTimes);

  int iTime = rModel.eventTimes ().size () - 1;
  Slice uOption = rModel.cash (iTime, 0.);

  double dMaturity = rAverTimes.back ();
  while (iTime > 0)
    {
      // uOption is the value of the sum of future spot prices
      uOption += rModel.spot (iTime) * rModel.discount (iTime, dMaturity);
      iTime--;
      uOption.rollback (iTime);
    }

  uOption /= (rModel.discount (iTime, dMaturity) * rAverTimes.size ());

  return interpolate (uOption);
}
