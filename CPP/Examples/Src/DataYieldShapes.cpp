#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"
#include "cfl/Macros.hpp"
#include <cmath>

using namespace cfl;
using namespace cfl::Data;
using namespace NExamples;

double
NExamples::shape1 (double dX)
{
  PRECONDITION (dX >= 0);

  double dY = (dX > cfl::EPS) ? (1. - std::exp (-dX)) / dX
                              : (1. - dX / 2. + dX * dX / 6.);

  return dY;
}

double
NExamples::shape2 (double dX)
{
  PRECONDITION (dX >= 0);

  double dY = (dX > cfl::EPS) ? (1. - std::exp (-dX) * (1. + dX)) / dX
                              : (dX / 2. - dX * dX / 3.);

  return dY;
}

cfl::Function
prb::yieldShape1 (double dLambda, double dInitialTime)
{
  std::function<double (double)> uShape = [dLambda, dInitialTime] (double dT) {
    PRECONDITION ((dLambda >= 0) && (dT >= dInitialTime));

    double dX = dLambda * (dT - dInitialTime);

    return shape1 (dX);
  };

  return Function (uShape, dInitialTime);
}

cfl::Function
prb::yieldShape2 (double dLambda, double dInitialTime)
{
  std::function<double (double)> uShape = [dLambda, dInitialTime] (double dT) {
    PRECONDITION ((dLambda >= 0) && (dT >= dInitialTime));

    double dX = dLambda * (dT - dInitialTime);

    return shape2 (dX);
  };

  return Function (uShape, dInitialTime);
}
