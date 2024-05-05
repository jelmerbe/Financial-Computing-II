#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;
using namespace prb;

cfl::Function
prb::discountNelsonSiegelFit (const std::vector<double> &rTimes,
                              const std::vector<double> &rDF, double dLambda,
                              double dInitialTime, Function &rErr,
                              FitParam &rParam)
{
  PRECONDITION (rTimes.front () > dInitialTime);
  PRECONDITION (
      is_sorted (rTimes.begin (), rTimes.end (), std::less_equal<double> ()));
  PRECONDITION (dLambda > 0);

  std::vector<Function> uF
      = { Function (1., dInitialTime), yieldShape1 (dLambda, dInitialTime),
          yieldShape2 (dLambda, dInitialTime) };
  Fit uFit = cfl::NFit::linear (uF);
  Function uD = discountYieldFit (rTimes, rDF, dInitialTime, uFit, rErr);
  rParam = uFit.param ();

  return uD;
}
