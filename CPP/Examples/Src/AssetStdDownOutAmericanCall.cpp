#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::downOutAmericanCall (double dLowerBarrier,
                          const std::vector<double> &rBarrierTimes,
                          double dStrike,
                          const std::vector<double> &rExerciseTimes,
                          AssetModel &rModel)
{
  PRECONDITION (rModel.initialTime () < rBarrierTimes.front ());
  PRECONDITION (rModel.initialTime () < rExerciseTimes.front ());
  PRECONDITION (rBarrierTimes.back () < rExerciseTimes.back ());
  PRECONDITION (std::is_sorted (rExerciseTimes.begin (), rExerciseTimes.end (),
                                std::less_equal<double> ()));
  PRECONDITION (std::is_sorted (rBarrierTimes.begin (), rBarrierTimes.end (),
                                std::less_equal<double> ()));

  std::vector<double> uEventTimes (1 + rBarrierTimes.size ()
                                   + rExerciseTimes.size ());
  uEventTimes.front () = rModel.initialTime ();
  std::vector<double>::iterator itEnd = std::set_union (
      rBarrierTimes.begin (), rBarrierTimes.end (), rExerciseTimes.begin (),
      rExerciseTimes.end (), uEventTimes.begin () + 1);
  uEventTimes.resize (itEnd - uEventTimes.begin ());
  rModel.assignEventTimes (uEventTimes);

  int iTime = uEventTimes.size () - 1;
  Slice uOption = rModel.cash (iTime, 0.);
  while (iTime > 0)
    {
      // uOption is the value to continue
      double dTime = uEventTimes[iTime];
      if (std::binary_search (rExerciseTimes.begin (), rExerciseTimes.end (),
                              dTime))
        {
          uOption = max (uOption, rModel.spot (iTime) - dStrike);
        }
      if (std::binary_search (rBarrierTimes.begin (), rBarrierTimes.end (),
                              dTime))
        {
          uOption *= indicator (rModel.spot (iTime), dLowerBarrier);
        }
      iTime--;
      uOption.rollback (iTime);
    }

  return interpolate (uOption);
}
