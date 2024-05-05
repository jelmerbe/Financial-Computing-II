#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::Function
prb::discountYieldFit (const std::vector<double> &rTime,
                       const std::vector<double> &rDF, double dInitialTime,
                       cfl::Fit &rFit, Function &rErr)
{
  PRECONDITION (rTime.size () == rDF.size ());
  PRECONDITION (rTime.size () > 0);
  PRECONDITION (rTime.front () > dInitialTime);
  PRECONDITION (std::is_sorted (rTime.begin (), rTime.end (),
                                std::less_equal<double> ()));

  std::vector<double> uYield (rTime.size ());
  std::transform (rTime.begin (), rTime.end (), rDF.begin (), uYield.begin (),
                  [dInitialTime] (double dT, double dD) {
                    ASSERT (dT - dInitialTime > cfl::EPS);

                    return -std::log (dD) / (dT - dInitialTime);
                  });
  rFit.assign (rTime.begin (), rTime.end (), uYield.begin ());

  Function uT ([dInitialTime] (double dT) { return dT - dInitialTime; },
               dInitialTime);
  Function uDF = exp (-rFit.fit () * uT);

  rErr = rFit.err () * uDF * uT;

  return uDF;
}
