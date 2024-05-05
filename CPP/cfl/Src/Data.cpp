#include "cfl/Data.hpp"
#include "cfl/Error.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>

using namespace cfl;

namespace cflData
{
std::function<bool (double)>
belongs (const cfl::Function &rF, double dInitialTime)
{
  return [rF, dInitialTime] (double dT) {
    return rF.belongs (dT) && (dT >= dInitialTime);
  };
}
} // namespace cflData

Function
cfl::Data::discount (double dYield, double dInitialTime)
{
  std::function<double (double)> uDiscount
      = [dYield, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          return std::exp (-dYield * (dT - dInitialTime));
        };

  return Function (uDiscount, dInitialTime);
}

Function
cfl::Data::discount (const Function &rYield, double dInitialTime)
{
  std::function<double (double)> uDiscount
      = [rYield, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          return std::exp (-rYield (dT) * (dT - dInitialTime));
        };

  return Function (uDiscount, cflData::belongs (rYield, dInitialTime));
}

// creation of volatility curves

Function
cfl::Data::volatility (double dSigma, double dLambda, double dInitialTime)
{
  PRECONDITION (dSigma >= 0);

  std::function<double (double)> uVol
      = [dSigma, dLambda, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          double dX = 2. * dLambda * (dT - dInitialTime);
          dX = (std::abs (dX) < cfl::EPS) ? 1. + 0.5 * dX
                                          : (std::exp (dX) - 1.) / dX;

          return dSigma * std::sqrt (dX);
        };

  return Function (uVol, dInitialTime);
}

// creation of forward curves

Function
cfl::Data::forward (double dSpot, double dCostOfCarry, double dInitialTime)
{
  std::function<double (double)> uForward
      = [dSpot, dCostOfCarry, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          return dSpot * std::exp (dCostOfCarry * (dT - dInitialTime));
        };

  return Function (uForward, dInitialTime);
}

Function
cfl::Data::forward (double dSpot, const Function &rCostOfCarry,
                    double dInitialTime)
{
  std::function<double (double)> uForward
      = [dSpot, rCostOfCarry, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          return dSpot * std::exp (rCostOfCarry (dT) * (dT - dInitialTime));
        };

  return Function (uForward, cflData::belongs (rCostOfCarry, dInitialTime));
}

Function
cfl::Data::forward (double dSpot, double dDividendYield,
                    const Function &rDiscount, double dInitialTime)
{
  std::function<double (double)> uForward
      = [dSpot, dDividendYield, rDiscount, dInitialTime] (double dT) {
          PRECONDITION (dT >= dInitialTime);

          return dSpot * std::exp (-dDividendYield * (dT - dInitialTime))
                 / rDiscount (dT);
        };

  std::function<bool (double)> uBelongs
      = [rDiscount, dInitialTime] (double dT) {
          return (dT >= dInitialTime) && rDiscount.belongs (dT);
        };

  return Function (uForward, uBelongs);
}

// CLASS: Swap

cfl::Data::Swap::Swap (const CashFlow &rCashFlow, bool bPayFloat)
    : CashFlow (rCashFlow), payFloat (bPayFloat)
{
}
