#include "test/Numeric.hpp"
#include <gsl/gsl_cdf.h>

using namespace cfl;
using namespace std;
using namespace test;

std::function<double (double)>
test::gauss_cdf (double dMean, double dSigma)
{
  return [dMean, dSigma] (double dX) {
    double dP = gsl_cdf_gaussian_P (dX - dMean, dSigma);
    return dP;
  };
}

std::function<unsigned (double)>
test::sizeApprox (double dQuality)
{
  return [dQuality] (double dX) {
    PRECONDITION (dX >= 0);
    unsigned iSize = 1;
    if (dX > 0)
      {
        double dQ = std::max<double> (dQuality, 3.);
        iSize = std::ceil (2.5 + (1. + dQ) * dX / (1. + dX));
        POSTCONDITION (iSize >= 3);
      }
    return iSize;
  };
}
