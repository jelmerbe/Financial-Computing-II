#ifndef __cflBlackModel_hpp__
#define __cflBlackModel_hpp__

/**
 * @file BlackModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Implementation of Black model.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Brownian.hpp"

namespace cfl
{
/**
 * @ingroup cflAssetModel
 *
 * @defgroup cflBlack Black model for a single asset.
 *
 * This module contains implementation of the classical
 * Black model for a single asset.
 *
 * @see cfl::AssetModel
 * @{
 */

/**
 * @brief  Black model for a single asset.
 *
 * This namespace contains functions and classes related with the
 * classical Black model for a single asset.
 */
namespace Black
{
/**
 * @brief  This class defines the parameters of the Black model.
 *
 * The set of parameters consists of discount, forward, shape, and
 * volatility curves. The forward prices evolve as
 * \f[
 *  dF(t,T) = F(t,T)A(T)\sigma(t) dW_t, \quad t\in [t_0,T],
 *  \quad F(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$F(t,T)\f$ is the forward price computed at \f$t\f$ for
 *   maturity \f$T\f$.
 * - \f$A(T)\f$ is the value of the shape function at maturity \f$T\f$,
 *   \f$A(t_0) = 1\f$.
 * - \f$\sigma(t)\f$ is the normalized  volatility at \f$t\f$.
 *   The true volatility for the forward price \f$F(t,T)\f$ is
 *   \f$A(T)\sigma(t)\f$.
 * - \f$t_0\f$ is the initial time.
 * - \f$W = (W_t)\f$ is a Brownian motion.
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
   * The initial forward curve \f$F(t_0,T)\f$, \f$T\geq t_0\f$.
   *
   */
  Function forward;

  /**
   * The initial shape curve \f$A(T)\f$, \f$T\geq t_0\f$,
   * \f$A(t_0)=1\f$.  It describes the form of changes in the
   * forward prices.
   *
   */
  Function shape;

  /**
   * The average normalized volatility curve
   * \f[
   * \Sigma(T) = \frac1{T-t_0} \sqrt{\int_{t_0}^T \sigma^2(t) dt},
   * \quad T\geq t_0.
   * \f]
   * The implied volatility for stock options expiring at
   * \f$T\f$ is \f$A(T)\Sigma(T)\f$.
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
 * Constructs the parameters of the classical Black model, where
 * shape function \f$A(t) = 1\f$. The forward prices evolve as
 * \f[
 * dF(t,T) = F(t,T) \sigma(t) dW_t, \quad t\in [t_0,T],
 * \quad F(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$F(t,T)\f$ is the forward price computed at \f$t\f$ for
 *   maturity \f$T\f$.
 * - \f$\sigma(t)\f$ is the  volatility at \f$t\f$.
 * - \f$t_0\f$ is the initial time.
 * - \f$W = (W_t)\f$ is a Brownian motion.
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rForward The initial forward curve \f$F(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rVolatility The  average volatility curve:
 * \f[
 * \Sigma(T) = \frac1{T-t_0} \sqrt{\int_{t_0}^T \sigma^2(t) dt},
 * \quad T\geq t_0.
 * \f]
 * It coincides with the implied volatility curve.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, const Function &rForward,
               const Function &rVolatility, double dInitialTime);

/**
 * Constructs the parameters of the classical Black model with
 * constant volatility \f$\sigma\f$ and shape function \f$A(t) =
 * 1\f$. The evolution of the forward prices has the form:
 * \f[
 * dF(t,T) = F(t,T) \sigma dW_t, \quad t\in [t_0,T],
 * \quad F(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$F(t,T)\f$ is the forward price computed at \f$t\f$ for
 *   maturity \f$T\f$.
 * - \f$\sigma\f$ is the constant  volatility.
 * - \f$t_0\f$ is the initial time.
 * - \f$W = (W_t)\f$ is a Brownian motion.
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rForward The initial forward curve \f$F(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param dSigma The constant volatility \f$\sigma\f$.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, const Function &rForward,
               double dSigma, double dInitialTime);

/**
 * Constructs the parameters of the general Black model. The
 * forward prices evolve as
 * \f[
 *  dF(t,T) = F(t,T)A(T)\sigma(t) dW_t, \quad t\in [t_0,T],
 *  \quad F(t_0,T) \text{ is given},
 * \f]
 * where
 * - \f$F(t,T)\f$ is the forward price computed at \f$t\f$ for
 *   maturity \f$T\f$.
 * - \f$A(T)\f$ is the value of the shape function at maturity \f$T\f$,
 *   \f$A(t_0) = 1\f$.
 * - \f$\sigma(t)\f$ is the normalized  volatility at \f$t\f$.
 *   The true  volatility for forward price \f$F(t,T)\f$ is
 *   \f$A(T)\sigma(t)\f$.
 * - \f$t_0\f$ is the initial time.
 * - \f$W = (W_t)\f$ is a Brownian motion.
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rForward The initial forward curve \f$F(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rVolatility  The average normalized volatility curve
 * \f[
 * \Sigma(T) = \frac1{T-t_0} \sqrt{\int_{t_0}^T \sigma^2(t) dt},
 * \quad T\geq t_0.
 * \f]
 * The implied volatility for stock options expiring at
 * \f$T\f$ is \f$A(T)\Sigma(T)\f$.
 * @param rShape The initial shape curve \f$A(T)\f$, \f$T\geq t_0\f$,
 * \f$A(t_0)=1\f$.  It describes the form of changes in the
 * forward prices.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, const Function &rForward,
               const Function &rVolatility, const Function &rShape,
               double dInitialTime);

/**
 * Constructs the parameters of general Black model with
 * stationary volatility curve determined by positive constants
 * \f$\kappa\f$ (the spot volatility) and \f$\lambda\f$ (the spot
 * mean-reversion). The forward prices evolve as
 * \f[
 *  dF(t,T) = F(t,T) e^{-\lambda(T-t)} \kappa dW_t, \quad t\in
 *  [t_0,T], \quad F(t_0,T) \text{ is given},
 * \f]
 * The shape function has the form:
 * \f[
 * A(t) = e^{-\lambda (t-t_0)}, \quad t\geq t_0.
 * \f]
 * The normalized volatility function is given by
 * \f[
 * \sigma(t) = \kappa e^{ \lambda (t-t_0)}, \quad t\geq t_0.
 * \f]
 * The implied volatility at \f$t\f$ for options expiring at
 * \f$T\f$ has the form:
 * \f[
 * \Psi(t,T) = \kappa \sqrt{\frac{1 - \exp(-2\lambda(T-t))}{2\lambda(T-t)}},
 * \quad t_0\leq t< T.
 * \f]
 *
 * @param rDiscount The initial discount curve \f$B(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param rForward The initial forward curve \f$F(t_0,T)\f$, \f$T\geq t_0\f$.
 * @param dKappa The volatility of the spot price.
 * @param dLambda The mean-reversion coefficient of the spot price.
 * @param dInitialTime The initial time given as year fraction.
 */
Data makeData (const Function &rDiscount, const Function &rForward,
               double dKappa, double dLambda, double dInitialTime);

/**
 * Implements AssetModel as Black model.
 *
 * @param rData The parameters of Black model.
 * @param dInterval The interval of initial values for relative
 * changes in the spot price.
 * @param dStepQuality The step quality of model implementation.
 * @param dWidthQuality The width quality of model implementation.
 * @param iUniformSteps The minimal number of uniform steps between
 * two event times.
 * @return An implementation of AssetModel  as Black model.
 */
AssetModel model (const Data &rData, double dInterval, double dStepQuality,
                  double dWidthQuality, unsigned iUniformSteps = 1);

/**
 * Implements AssetModel as Black model.
 *
 * @param rData The parameters of Black model.
 * @param dInterval The interval of initial values for relative
 * changes in the spot price.
 * @param rBrownian Constructor of Brownain model.
 * @return An implementation of AssetModel  as Black model.
 */
AssetModel model (const Data &rData, double dInterval,
                  const TBrownian &rBrownian);
} // namespace Black
/** @} */
} // namespace cfl

#include "cfl/Inline/iBlackModel.hpp"
#endif // of __cflBlackModel_hpp__
