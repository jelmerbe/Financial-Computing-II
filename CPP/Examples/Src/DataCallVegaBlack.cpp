#include "Examples/Examples.hpp"
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>

using namespace cfl;
using namespace std;

double
prb::callVegaBlack (double dK, double dT, double dDF, double dF, double dSigma)
{
  PRECONDITION ((dK > 0) && (dT > 0) && (dDF > 0) && (dF > 0) && (dSigma > 0));

  double dX = dSigma * std::sqrt (dT);

  ASSERT (dX > cfl::EPS);

  double d1 = std::log (dF / dK) / dX + 0.5 * dX;
  double dVega = dDF * dF * sqrt (dT) * gsl_ran_ugaussian_pdf (d1);

  ASSERT (abs (dVega - dDF * dK * sqrt (dT) * gsl_ran_ugaussian_pdf (d1 - dX))
          < cfl::EPS);

  return dVega;
}
