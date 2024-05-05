#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::americanPut (double dStrike, const std::vector<double> &rExerciseTimes,
                  AssetModel &rModel)
{
  PRECONDITION (rModel.initialTime () < rExerciseTimes.front ());
  PRECONDITION (std::is_sorted (rExerciseTimes.begin (), rExerciseTimes.end (),
                                std::less_equal<double> ()));

  std::vector<double> uEventTimes (rExerciseTimes.size () + 1);
  uEventTimes.front () = rModel.initialTime ();
  copy (rExerciseTimes.begin (), rExerciseTimes.end (),
        uEventTimes.begin () + 1);
  rModel.assignEventTimes (uEventTimes);

  int iTime = uEventTimes.size () - 1;
  Slice uOption = rModel.cash (iTime, 0.);
  while (iTime > 0)
    {
      // uOption is the value to continue
      uOption = max (uOption, dStrike - rModel.spot (iTime));
      iTime--;
      uOption.rollback (iTime);
    }

  return interpolate (uOption);
}
