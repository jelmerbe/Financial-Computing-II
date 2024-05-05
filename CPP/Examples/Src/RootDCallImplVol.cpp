#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

double
prb::callImplVolBlack (double dK, double dT, double dDF, double dF, double dC,
                       double dSigma0, const cfl::RootD &rSolver)
{
  Function uF ([dK, dT, dDF, dF, dC] (double dSigma) {
    return callBlack (dK, dT, dDF, dF, dSigma) - dC;
  });

  Function uDF ([dK, dT, dDF, dF] (double dSigma) {
    return callVegaBlack (dK, dT, dDF, dF, dSigma);
  });

  return rSolver.find (uF, uDF, dSigma0);
}
