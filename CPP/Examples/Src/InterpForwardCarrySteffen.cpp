#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::Function
prb::forwardCarrySteffenInterp (double dSpot,
                                const std::vector<double> &rTimes,
                                const std::vector<double> &rForwards,
                                double dInitialTime)
{
  // initial cost-of-carry rate
  double dF = rForwards.front ();
  double dT = rTimes.front ();

  ASSERT (dT > dInitialTime);

  double dC0 = log (dF / dSpot) / (dT - dInitialTime);
  cfl::Interp uInterp = cfl::NInterp::steffen ();

  return forwardCarryInterp (dSpot, rTimes, rForwards, dC0, dInitialTime,
                             uInterp);
}
