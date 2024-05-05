#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::barrierUpDownOut (double dNotional, double dLowerBarrier,
                       double dUpperBarrier,
                       const std::vector<double> &rBarrierTimes,
                       AssetModel &rModel)
{
  PRECONDITION (rModel.initialTime () < rBarrierTimes.front ());
  PRECONDITION (dLowerBarrier < dUpperBarrier);
  PRECONDITION (std::is_sorted (rBarrierTimes.begin (), rBarrierTimes.end (),
                                std::less_equal<double> ()));

  std::vector<double> uEventTimes (rBarrierTimes.size () + 1);
  uEventTimes.front () = rModel.initialTime ();
  copy (rBarrierTimes.begin (), rBarrierTimes.end (),
        uEventTimes.begin () + 1);
  rModel.assignEventTimes (uEventTimes);

  int iTime = uEventTimes.size () - 1;
  Slice uOption = rModel.cash (iTime, dNotional);
  while (iTime > 0)
    {
      // uOption is the value to continue (the value of the option
      // if no barriers have been crossed before and now)
      uOption *= indicator (rModel.spot (iTime), dLowerBarrier);
      uOption *= indicator (dUpperBarrier, rModel.spot (iTime));
      iTime--;
      uOption.rollback (iTime);
    }

  return interpolate (uOption);
}
