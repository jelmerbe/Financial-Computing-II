#include "test/Parameters.hpp"
#include "test/Output.hpp"

using namespace std;

std::vector<double>
test::exerciseTimes ()
{
  unsigned iExerciseTimes = 12;
  double dFactor = 1. - 1. / double (iExerciseTimes);
  double dExerciseMaturity = c_dMaturity * dFactor;
  return test::getTimes (c_dInitialTime + c_dMaturity * (1. - dFactor),
                         dExerciseMaturity, iExerciseTimes);
}

std::vector<double>
test::barrierTimes ()
{
  unsigned iBarrierTimes = 10;
  double dFactor = 1. - 1. / double (iBarrierTimes);
  double dBarrierMaturity = c_dMaturity * dFactor;
  return test::getTimes (c_dInitialTime, dBarrierMaturity, iBarrierTimes);
}

cfl::Data::Swap
test::swapParameters ()
{
  cfl::Data::Swap uSwap;
  uSwap.notional = c_dNotional;
  uSwap.rate = c_dYield;
  uSwap.period = c_dPeriod;
  uSwap.numberOfPayments = c_iNumberOfPeriods;
  uSwap.payFloat = true;
  return uSwap;
}
