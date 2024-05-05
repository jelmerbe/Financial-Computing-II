#ifndef __Session3_hpp__
#define __Session3_hpp__

/**
 * @file Session3.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Session 3: least-squares fit of data curves
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/Fit.hpp"
#include "cfl/Interp.hpp"

/**
 * @mainpage Session 3: least-squares fit of data curves.
 */

namespace prb
{
/**
 * @defgroup Session3Fit Least-squares fit of data curves.
 *
 * This module constructs input data curves for financial
 * models using least-squares fitting.
 * @{
 */

/**
 * Returns discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * by approximate least-squares fit of market discount factors. More
 * precisely, yield curve \f$\gamma=\gamma(t)\f$ is the result of
 * the minimization:
 * \f[
 * \sum_{i=1}^{M} w_i(\gamma(t_i) - \gamma_i)^2  \to \min,
 * \f]
 * with method \p rFit and weights
 * \f[
 * w_i = d_i^2 (t_i-t_0)^2, \quad i=1,\dots,M,
 * \f]
 * where \f$\gamma_i\f$ is market's continuously compounded yield
 * for maturity \f$t_i\f$. This expression is the first order
 * approximation to the (non-linear) least-squares fitting of
 * discount curve \f$d=d(t)\f$ to market discount factors
 * \f$(d_i)\f$.
 *
 * @param rDiscountTimes (\f$(t_i)_{i=1}^{M}\f$) The maturities for market
 * discount factors.
 * @param rDiscountFactors (\f$(d_i)_{i=1}^M\f$) The market discount factors.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for discount factors.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function discountApproxFit (const std::vector<double> &rDiscountTimes,
                                 const std::vector<double> &rDiscountFactors,
                                 double dInitialTime, cfl::Fit &rFit,
                                 cfl::Function &rErr);

/**
 * Returns forward curve in the form:
 * \f[
 * F(t) = S_0 \exp(q(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ is obtained by the
 * least-squares fit of the market cost-of-carry rates with method
 * \p rFit.
 *
 * @param dSpot (\f$S_0\f$) The spot price of the stock.
 * @param rDeliveryTimes The delivery times of forward contracts.
 * @param rForwardPrices The market forward prices.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for forward prices.
 *
 * @return The fitted forward curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function forwardCarryFit (double dSpot,
                               const std::vector<double> &rDeliveryTimes,
                               const std::vector<double> &rForwardPrices,
                               double dInitialTime, cfl::Fit &rFit,
                               cfl::Function &rErr);

/**
 * Returns the forward curve in the form:
 * \f[
 * F(t) = S_0 \exp(q(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ fits the market
 * cost-of-carry rates in the Black model for commodities:
 * \f[
 * q(t) = \theta \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)} +
 * \frac{\sigma^2}2 \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)},
 * \quad t\geq t_0.
 * \f]
 *
 * @param dSpot (\f$S_0\f$) The spot price of the stock.
 * @param rTimes The maturities of forward contracts.
 * @param rForwards The market forward prices.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dSigma (\f$\sigma>0\f$) The volatility.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for forward prices.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted forward curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function forwardBlackFit (double dSpot, const std::vector<double> &rTimes,
                               const std::vector<double> &rForwards,
                               double dLambda, double dSigma,
                               double dInitialTime, cfl::Function &rErr,
                               cfl::FitParam &rParam);

/**
 * Returns the forward curve in the form:
 * \f[
 * F(t) = S_0 \exp(q(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ fits the market
 * cost-of-carry rates in the multi-factor Black model for
 * commodities:
 * \f[
 *  q(t) = \sum_{j=0}^N \left(\theta_j
 * \frac{1-e^{-\lambda_j(t-t_0)}}{\lambda_j(t-t_0)} +
 * \frac{\sigma_j^2}2
 * \frac{1-e^{-2\lambda_j(t-t_0)}}{2\lambda_j(t-t_0)}\right). \quad t\geq t_0.
 * \f]
 *
 * @param dSpot (\f$S_0 = S(t_0)\f$) The spot price of the stock.
 * @param rTimes The maturities of forward contracts.
 * @param rForwards The market forward prices.
 * @param rLambda \f$(\lambda_i\geq 0)\f$ The mean-reversion rates.
 * @param rSigma \f$(\sigma_i>0)\f$ The volatilities.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for forward prices.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted forward curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function forwardBlackMultiFit (double dSpot,
                                    const std::vector<double> &rTimes,
                                    const std::vector<double> &rForwards,
                                    const std::vector<double> &rLambda,
                                    const std::vector<double> &rSigma,
                                    double dInitialTime, cfl::Function &rErr,
                                    cfl::FitParam &rParam);

/** @} */
}

#endif // of __Session3_hpp__
