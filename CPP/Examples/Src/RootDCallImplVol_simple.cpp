#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

double
prb::callImplVolBlack (double dK, double dT, double dDF, double dF, double dC,
                       double dSigma0, double dErr)
{
  PRECONDITION ((dK > 0) && (dT > 0) && (dDF > 0) && (dF > 0) && (dC > 0)
                && (dSigma0 > 0) && (dErr > 0));

  double dSigma = dSigma0;
  double dDiff = callBlack (dK, dT, dDF, dF, dSigma) - dC;

  while (abs (dDiff) > dErr)
    {
      dSigma -= dDiff / callVegaBlack (dK, dT, dDF, dF, dSigma);
      dDiff = callBlack (dK, dT, dDF, dF, dSigma) - dC;
    }

  return dSigma;
}
