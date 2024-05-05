#ifndef __Homework2_hpp__
#define __Homework2_hpp__

/**
 * @file Homework2.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Homework 2: root finding and interpolation
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Root.hpp"

/**
 * @mainpage Homework 2: root finding and interpolation
 */

namespace prb
{
/**
 * @defgroup Homework2Root Root finding.
 *
 * This module deals with algorithms for one-dimensional root
 * finding.
 *
 * @{
 */

/**
 * Computes the continuously compounded yield to maturity (YTM) for
 * a cash flow using a root bracketing algorithm. Yield to maturity
 * \f$\gamma\f$ solves the equation:
 * \f[
 *   S(t_0) = \sum_{i=1}^M P_ie^{-\gamma(t_i-t_0)},
 * \f]
 * where \f$(P_i)_{i=1,\dots,M}\f$ and \f$(t_i)_{i=1,\dots,M}\f$ are
 * the payments and the payment times, respectively, \f$t_1>t_0\f$.
 *
 * @param rPayments (\f$(P_i)\f$) The vector of payments.
 * @param rPaymentTimes (\f$(t_i)\f$) The vector of payment times,
 * \f$t_1>t_0\f$.
 * @param dValue (\f$S(t_0)\f$) The current value of the payments.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param dY0 The lower bound for the YTM.
 * @param dY1 The upper bound for the YTM.
 * @param rRoot The root bracketing algorithm.
 *
 * @return The YTM of the payments.
 */
double yieldTMCashFlow (const std::vector<double> &rPayments,
                        const std::vector<double> &rPaymentTimes,
                        double dValue, double dInitialTime, double dY0,
                        double dY1, const cfl::Root &rRoot);

/**
 * Computes the curve of forward yields to maturity for given cash
 * flow. For \f$t\in [t_0,t_M)\f$, the forward yield to maturity
 * \f$\gamma(t)\f$ solves the equation:
 * \f[
 *  F(t) = \sum_{t_i>t} P_ie^{-\gamma(t)(t_i-t)},
 * \f]
 * where \f$F(t)\f$ is the forward price computed at \f$t_0\f$ for
 * delivery at \f$t\f$. In the forward contract, the buyer pays
 * forward price \f$F(t)\f$ at delivery time \f$t\f$ and then receives
 * payments \f$(P_i)\f$ at payments times \f$t_i>t\f$.
 *
 * @param rPayments (\f$(P_i)\f$) The vector of payments.
 * @param rPaymentTimes (\f$(t_i)\f$) The vector of payment times,
 * \f$t_1>t_0\f$.
 * @param rForward (\f$(F(t))\f$) The forward price curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param dY0 The lower bound for the YTM.
 * @param dY1 The upper bound for the YTM.
 * @param rRoot The root bracketing algorithm.
 *
 * @return The forward price curve of a cash flow.
 */
cfl::Function forwardYTMCashFlow (const std::vector<double> &rPayments,
                                  const std::vector<double> &rPaymentTimes,
                                  const cfl::Function &rForward,
                                  double dInitialTime, double dY0, double dY1,
                                  const cfl::Root &rRoot);

/** @} */

/**
 * @defgroup prbInterp Interpolation of data curves.
 *
 * This module constructs input data curves for financial
 * models using interpolation.
 * @{
 */

/**
 * Computes implied volatility curve \f$\Sigma = \Sigma(t)\f$ by
 * applying interpolation method \p rInterp to variance curve
 * \f[
 *      V(t) = \Sigma^2(t)(t - t_0), \quad t\geq t_0.
 * \f]
 * The resulting volatility curve is constant on \f$[t_0,t_1]\f$.
 *
 * @param rTimes (\f$(t_i)_{i=1,\dots,M}\f$) The maturities of the
 * market implied volatilities, (\f$t_1>t_0\f$.
 * @param rVols The market implied volatilities.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rInterp A monotone interpolation method.
 *
 * @return cfl::Function The implied volatility curve obtained by
 * the interpolation of market variances.
 */
cfl::Function volatilityVarInterp (const std::vector<double> &rTimes,
                                   const std::vector<double> &rVols,
                                   double dInitialTime, cfl::Interp &rInterp);

/**
 * Computes implied volatility curve \f$\Sigma = \Sigma(t)\f$ by
 * applying Steffen interpolation to variance curve
 * \f[
 *      V(t) = \Sigma^2(t)(t - t_0), \quad t\geq t_0.
 * \f]
 *
 * @param rTimes (\f$(t_i)_{i=1,\dots,M}\f$) The maturities of the
 * market implied volatilities, (\f$t_1>t_0\f$.
 * @param rVols The vector of market volatilities.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The volatility curve obtained by Steffen interpolation
 * of market variances.
 */
cfl::Function volatilityVarSteffenInterp (const std::vector<double> &rTimes,
                                          const std::vector<double> &rVols,
                                          double dInitialTime);

/** @} */
}

#endif // of __Homework2_hpp__
