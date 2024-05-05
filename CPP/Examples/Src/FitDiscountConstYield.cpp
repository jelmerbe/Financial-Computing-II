#include "Examples/Examples.hpp"
#include <numeric>

using namespace cfl;
using namespace std;
using namespace prb;

cfl::Function
prb::discountConstYieldFit (const std::vector<double> &rTimes,
                            const std::vector<double> &rDiscount,
                            double dInitialTime, cfl::Function &rErr,
                            cfl::FitParam &rParam)
{
  PRECONDITION (rTimes.front () > dInitialTime);
  PRECONDITION (
      is_sorted (rTimes.begin (), rTimes.end (), std::less_equal<double> ()));

  cfl::Fit uYieldFit = cfl::NFit::linear (Function (1., dInitialTime));
  Function uFit
      = discountYieldFit (rTimes, rDiscount, dInitialTime, uYieldFit, rErr);
  rParam = uYieldFit.param ();

  return uFit;
}
