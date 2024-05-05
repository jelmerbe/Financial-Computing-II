#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"
#include <numeric>

using namespace cfl;
using namespace std;
using namespace prb;
using namespace NExamples;

double
vasicekH (double dL, double dSigma, double dZ)
{
  PRECONDITION ((dL > cfl::EPS) & (dZ >= 0));

  double dX = dL * dZ;
  double dA = shape1 (dX);
  double dB = shape1 (2. * dX);
  double dR = dSigma / dL;
  double dY = -0.5 * dR * dR * (1. - 2. * dA + dB);

  return dY;
}

cfl::Function
prb::discountVasicekFit (const std::vector<double> &rTimes,
                         const std::vector<double> &rDF, double dLambda,
                         double dSigma, double dInitialTime,
                         cfl::Function &rErr, cfl::FitParam &rParam)
{
  PRECONDITION (dLambda > 0);
  PRECONDITION (rTimes.front () > dInitialTime);

  // basis functions, free function, and fitting engine
  double dL = max (dLambda, cfl::EPS);
  Function uF1 = yieldShape1 (dL, dInitialTime);
  Function uF2 = (1. - uF1) / dL;
  std::vector<Function> uBase = { uF1, uF2 };
  Function uH (
      [dL, dSigma, dInitialTime] (double dT) {
        return vasicekH (dL, dSigma, dT - dInitialTime);
      },
      dInitialTime);
  cfl::Fit uFit = cfl::NFit::linear (uBase, uH);

  Function uDF = discountYieldFit (rTimes, rDF, dInitialTime, uFit, rErr);
  rParam = uFit.param ();

  return uDF;
}
