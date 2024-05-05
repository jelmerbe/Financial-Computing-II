#ifndef __cflHullWhiteModel_hpp__
#define __cflHullWhiteModel_hpp__

/**
 * @file HullWhiteModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Implementation of Hull and White model.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/Brownian.hpp"
#include "cfl/Data.hpp"
#include "cfl/InterestRateModel.hpp"

namespace cfl
{
/**
 * @ingroup cflInterestRateModel
 *
 * @defgroup cflHullWhite Hull and White model for interest rates.
 *
 * This module contains implementation of the classical
 * Hull and White model for interest rates.
 *
 * @see cfl::InterestRateModel
 * @{
 */

/**
 * @brief  Hull and White model for interest rates.
 *
 * This namespace contains classes and functions related to the
 * Hull and White model for interest rates.
 */
namespace HullWhite
{

/**
 * @brief  The parameters of Hull and White model.
 *
 * This class defines the parameters of the Hull-White model
 * for interest rates. The set of parameters consists of
 * discount, shape, and volatility curves. We denote
 * - \f$B(t,T)\f$ the discount factor computed at \f$t\f$ for
 *   maturity \f$T\f$,
 * - \f$F(t,T,U) = B(t,U)/B(t,T)\f$ the forward price
 *    at \f$t\f$ in the contract expiring at \f$T\f$ and written
 *    on the discount factor with maturity \f$U>T\f$.
 *
 * The forward prices evolve as
 * \f[
 *  dF(t,T,U) = F(t,T,U)(A(U)-A(T))\sigma(t) dW^T_t, \quad t\in [t_0,T],
 *  \quad F(t_0,T,U) = B(t_0,U)/B(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$A=A(T)\f$, \f$T\geq t_0\f$, is the shape function,
 *   \f$A(t_0) = 0\f$, \f$A'(t_0) = 1\f$.
 * - \f$\sigma(t)\f$ is the normalized volatility at \f$t\f$.  The
 *   true volatility for forward price \f$F(t,T,U)\f$ is
 *   \f$(A(U)-A(T))\sigma(t)\f$.
 * - \f$t_0\f$ is the initial time.
 * - \f$W^T = (W^T_t)\f$ is a Brownian motion under the forward
 *   martingale measure corresponding to maturity \f$T\f$.
 */
class Data
{
public:
  /**
   * The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
   *
   */
  Function discount;

  /**
   * The initial shape curve \f$A(T)\f$, \f$T\geq t_0\f$, \f$A(t_0) =
   *  0\f$, \f$A'(t_0) = 1\f$. It describes the form of changes in
   *  the yield and discount curves.
   *
   */
  Function shape;

  /**
   * The average normalized volatility curve
   * \f[
   * \Sigma(T) = \frac1{T-t_0} \sqrt{\int_{t_0}^T \sigma^2(t) dt},
   * \quad T\geq t_0.
   * \f]
   * The implied volatility of options expiring at \f$T\f$ and
   * written on the discount factor with maturity \f$U>T\f$ is
   * \f$(A(U)-A(T))\Sigma(T)\f$.
   *
   */
  Function volatility;

  /**
   * The initial time \f$t_0\f$ given as year fraction.
   *
   */
  double initialTime;
};

/**
 * Constructs the parameters of the Hull-White model for
 * interest rates. We denote
 * - \f$B(t,T)\f$ the discount factor computed at \f$t\f$ for
 *   maturity \f$T\f$,
 * - \f$F(t,T,U) = B(t,U)/B(t,T)\f$ the forward price
 *    at \f$t\f$ in the contract expiring at \f$T\f$ and written
 *    on the discount factor with maturity \f$U>T\f$.
 *
 * The forward prices evolve as
 * \f[
 *  dF(t,T,U) = F(t,T,U)(A(U)-A(T))\sigma(t) dW^T_t, \quad t\in [t_0,T],
 *  \quad F(t_0,T,U) = B(t_0,U)/B(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$A=A(T)\f$, \f$T\geq t_0\f$, is the shape function,
 *   \f$A(t_0) = 0\f$, \f$A'(t_0) = 1\f$.
 * - \f$\sigma(t)\f$ is the normalized volatility at \f$t\f$.  The
 *   true volatility for forward price \f$F(t,T,U)\f$ is
 *   \f$(A(U)-A(T))\sigma(t)\f$.
 * - \f$t_0\f$ is the initial time.
 * - \f$W^T = (W^T_t)\f$ is a Brownian motion under the forward
 *   martingale measure corresponding to maturity \f$T\f$.
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$,
 * \f$T\geq t_0\f$.
 * @param rVolatility  The average normalized volatility curve
 * \f[
 * \Sigma(T) = \frac1{T-t_0} \sqrt{\int_{t_0}^T \sigma^2(t) dt},
 * \quad T\geq t_0.
 * \f]
 * The implied volatility of options expiring at \f$T\f$ and
 * written on the discount factor with maturity \f$U>T\f$ is
 * \f$(A(U)-A(T))\Sigma(T)\f$.
 * @param rShape The initial shape curve \f$A(T)\f$, \f$T\geq t_0\f$, \f$A(t_0)
 * = 0\f$, \f$A'(t_0) = 1\f$. It describes the form of changes in the yield and
 * discount curves.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, const Function &rVolatility,
               const Function &rShape, double dInitialTime);

/**
 * Constructs the parameters of the Hull-White model for interest
 * rates with stationary volatility curve determined by positive
 * constants \f$\kappa\f$ (the volatility of the short rate) and
 * \f$\lambda\f$ (the mean-reversion of the short rate). We denote
 * - \f$B(t,T)\f$ the discount factor computed at \f$t\f$ for
 *   maturity \f$T\f$,
 * - \f$F(t,T,U) = B(t,U)/B(t,T)\f$ the forward price
 *    at \f$t\f$ in the contract expiring at \f$T\f$ and written
 *    on the discount factor with maturity \f$U>T\f$.
 *
 * The forward prices evolve as
 * \f[
 *  dF(t,T,U) = F(t,T,U)(e^{-\lambda (U-t)} - e^{-\lambda (T-t)})
 * \frac{\kappa}{\lambda} dW^T_t, \quad t\in [t_0,T],
 *  \quad F(t_0,T,U) = B(t_0,U)/B(t_0,T) \text{ is given},
 * \f]
 * The shape function has the form:
 * \f[
 * A(t) = \frac{1-e^{-\lambda (t-t_0)}}{\lambda}, \quad t\geq t_0.
 * \f]
 * The normalized volatility function is given by
 * \f[
 * \sigma(t) = \kappa e^{ \lambda (t-t_0)}, \quad t\geq t_0.
 * \f]
 * The average normalized volatility curve has the form:
 * \f[
 * \Sigma(T) = \kappa\sqrt{\frac{\exp(2\lambda(T-t_0)-1}{2\lambda(T-t_0)}},
 * \quad T\geq t_0.
 * \f]
 * The implied volatility at \f$t\f$ for options expiring at
 * \f$T\f$ written on the discount factor with maturity \f$U>T\f$
 * has the form:
 * \f[
 * \Psi(t,T,U) = \kappa \frac{1-\exp(-\lambda(U-T))}{\lambda}
 * \sqrt{\frac{1 - \exp(-2\lambda(T-t))}{2\lambda(T-t)}},
 * \quad t_0\leq t< T.
 * \f]
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param dKappa The volatility of the spot price.
 * @param dLambda The mean-reversion coefficient of the spot price.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, double dKappa, double dLambda,
               double dInitialTime);

/**
 * Implements \p InterestRateModel as Hull and White model.
 *
 * @param rData The parameters of Hull and White model.
 * @param dInterval The interval of initial values for short-term
 * interest rate.
 * @param dStepQuality The step quality of model implementation.
 * @param dWidthQuality The width quality of model implementation.
 * @param iUniformSteps The minimal number of uniform steps between
 * two event times.
 * @return \p InterestRateModel as Hull and White model.
 */
InterestRateModel model (const Data &rData, double dInterval,
                         double dStepQuality, double dWidthQuality,
                         unsigned iUniformSteps = 5);

/**
 * Implements \p InterestRateModel as Hull and White model.
 *
 * @param rData The parameters of Hull and White model.
 * @param dInterval The interval of initial values for short-term
 * interest rate.
 * @param rBrownian A constructor of Brownian model.
 * @return \p InterestRateModel as Hull and White model.
 */
InterestRateModel model (const Data &rData, double dInterval,
                         const TBrownian &rBrownian);
} // namespace HullWhite
/** @} */
} // namespace cfl

#include "cfl/Inline/iHullWhiteModel.hpp"
#endif // of __cflHullWhiteModel_hpp__
