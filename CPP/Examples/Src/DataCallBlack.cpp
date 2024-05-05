#include "Examples/Examples.hpp"
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>

using namespace cfl;
using namespace std;

double
prb::callBlack (double dK, double dT, double dDF, double dF, double dSigma)
{
  PRECONDITION ((dK > 0) && (dT > 0) && (dDF > 0) && (dF > 0) && (dSigma > 0));

  double dX = dSigma * std::sqrt (dT);

  ASSERT (dX > cfl::EPS);

  double d1 = std::log (dF / dK) / dX + 0.5 * dX;
  double d2 = d1 - dX;

  ASSERT (
      abs (dF * gsl_ran_ugaussian_pdf (d1) - dK * gsl_ran_ugaussian_pdf (d2))
      < cfl::EPS);

  double dC
      = dDF * (dF * gsl_cdf_ugaussian_P (d1) - dK * gsl_cdf_ugaussian_P (d2));

  return dC;
}
