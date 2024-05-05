#ifndef __Review1_hpp__
#define __Review1_hpp__

/**
 * @file Review1.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Review 1: data curves for financial models.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/Fit.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Root.hpp"
#include "cfl/RootD.hpp"

/**
 * @mainpage Review 1: data curves for financial models.
 */

namespace prb
{
/**
 * @defgroup Review1Data Data structures.
 *
 * This module constructs input data curves for financial models.
 * @{
 */

/**
 * Computes yield curve \f$\gamma=(\gamma(t))_{t\geq t_0}\f$ for the Vasicek
 * model:
 * \f[
 * \gamma(t) = r(t_0) A(t) + \frac{\theta}{\lambda}(1-A(t)) -
 * \frac{\sigma^2}{2\lambda^2} (1-2A(t)+B(t)),
 * \quad t\geq t_0,
 * \f]
 * where
 * \f[
 * A(t) = \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)}, \quad B(t)
 * = \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)}.
 * \f]
 *
 * @param dTheta (\f$\theta\f$) The drift.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dSigma (\f$\sigma\geq 0\f$) The volatility.
 * @param dR0 (\f$r(t_0)\f$) The initial short-term interest rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return Yield curve for the Vasicek model of interest rates.
 */
cfl::Function yieldVasicek (double dTheta, double dLambda, double dSigma,
                            double dR0, double dInitialTime);

/**
 * Computes the curve of forward prices ("clean" or "dirty") for an
 * annuity. The annuity pays coupons \f$q \delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
 *     t_M = T.
 * \f]
 * The buyer pays forward price \f$F(t)\f$ at delivery time \f$t\f$
 * and then receives coupons \f$q\delta t\f$ at payments times
 * \f$t_i>t\f$.
 *
 * @param dRate (\f$q\f$) The annuity rate.
 * @param dPeriod (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity (\f$T\f$) The maturity.
 * @param rDiscount The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param bClean This parameter controls the output. If its value
 * is \p true, then we compute "clean" prices. Otherwise, we
 * compute "dirty" prices that include the accrued interest from
 * the next coupon.
 *
 * @return The forward prices ("clean" or "dirty") for an annuity.
 */
cfl::Function forwardAnnuity (double dRate, double dPeriod, double dMaturity,
                              const cfl::Function &rDiscount,
                              double dInitialTime, bool bClean);
/** @} */

/**
 * @defgroup Review1Root Root finding.
 *
 * This module deals with algorithms for one-dimensional root
 * finding.
 *
 * @{
 */

/**
 * Computes the continuously compounded YTM for an annuity. The
 * annuity pays coupons \f$q\delta t\f$ at times \f$(t_i)_{i=1,\dots,M}\f$
 * such that
 * \f[
 *  t_0<t_1\leq t_0+\delta t, \quad t_{i+1}-t_i = \delta t,\quad
 *  t_M = T.
 * \f]
 * Yield to maturity \f$\gamma\f$ solves the equation:
 * \f[
 * S(t_0) = q\delta t \sum_{i=1}^M e^{-\gamma(t_i-t_0)}.
 * \f]
 *
 * @param dRate (\f$q\f$) The annuity rate.
 * @param dPeriod (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity (\f$T\f$) The maturity.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param dPrice (\f$S(t_0)\f$) The current price of the annuity.
 * @param dYield0 The initial guess of the YTM.
 * @param rRootD The root finding method of polishing type.
 *
 * @return The YTM of the annuity.
 */
double yieldTMAnnuity (double dRate, double dPeriod, double dMaturity,
                       double dInitialTime, double dPrice, double dYield0,
                       const cfl::RootD &rRootD);

/**
 * Computes the curve of forward yields to maturity for an annuity.
 * The annuity pays coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
 *     t_M = T.
 * \f]
 * For \f$t\in [t_0,T)\f$, the forward yield to maturity
 * \f$\gamma(t)\f$ solves the equation:
 * \f[
 *  F(t) = q\delta t \sum_{t_i>t}
 * e^{-\gamma(t)(t_i-t)},
 * \f]
 * where \f$F(t)\f$ is the "dirty" forward price computed at
 * \f$t_0\f$ for delivery at \f$t\f$. In the forward contract, the
 * buyer pays forward price \f$F(t)\f$ at delivery time \f$t\f$ and
 * then receives coupons \f$q\delta t\f$ at payments times
 * \f$t_i>t\f$.
 *
 * @param dRate (\f$q\f$) The annuity rate.
 * @param dPeriod (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity  (\f$T\f$) The maturity.
 * @param rDiscount The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rRoot The root finding routing of polishing type.
 *
 * @return The curve of forward yields to maturity for the annuity.
 */
cfl::Function forwardYTMAnnuity (double dRate, double dPeriod,
                                 double dMaturity,
                                 const cfl::Function &rDiscount,
                                 double dInitialTime, const cfl::RootD &rRoot);
/** @} */

/**
 * @defgroup Review1Interp Interpolation of data curves.
 *
 * This module constructs input data curves for financial
 * models using interpolation.
 * @{
 */
/**
 * Computes forward curve by applying the log-linear interpolation to
 * the market spot and forward prices.
 *
 * @param dSpot The spot price of the stock.
 * @param rDeliveryTimes The delivery times for market forward
 * prices.
 * @param rForwardPrices The  market forward prices.
 * @param dInitialTime The initial time.
 *
 * @return The forward curve obtained from the market spot and
 * forward prices by log-linear interpolation.
 */
cfl::Function forwardLogLinInterp (double dSpot,
                                   const std::vector<double> &rDeliveryTimes,
                                   const std::vector<double> &rForwardPrices,
                                   double dInitialTime);

/**
 * Computes forward curve
 * \f[
 * F(t) = S_0 e^{q(t)(t-t_0)}, \quad t\in [t_0,t_M],
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ is obtained by the linear
 * interpolation to the market cost-of-carry rates. On the first
 * interval \f$[t_0,t_1]\f$, cost-of-carry function \f$q=q(t)\f$ is
 * assumed to be constant.
 *
 * @param dSpotFX (\f$S_0\f$) The spot exchange rate.
 * @param rDiscountTimes (\f$(t_i)_{i=1,\dots,M}\f$) The maturities of
 * discount factors.
 * @param rDomesticDiscountFactors The domestic discount factors.
 * @param rForeignDiscountFactors The foreign discount factors.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The forward exchange rate curve obtained by the linear
 * interpolation of the market cost-of-carry rates.
 */
cfl::Function forwardFXCarryLinInterp (
    double dSpotFX, const std::vector<double> &rDiscountTimes,
    const std::vector<double> &rDomesticDiscountFactors,
    const std::vector<double> &rForeignDiscountFactors, double dInitialTime);
/** @} */

/**
 * @defgroup Review1Fit Least-squares fitting of data curves.
 *
 * This module constructs input data curves for financial
 * models using least-squares fitting.
 * @{
 */

/**
 * Returns the forward curve in the form:
 * \f[
 * F(t) = S_0 \exp(q(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ fits the market
 * cost-of-carry rates in the 2-factor Black model for commodities:
 * \f[
 * q(t) = \theta_0 + \theta_1 \frac{1-e^{-\lambda_1 (t-t_0)}}{\lambda_1
 * (t-t_0)} + \frac{\sigma_0^2}{2} +
 * \frac{\sigma_1^2}2 \frac{1-e^{-2\lambda_1 (t-t_0)}}{2\lambda_1 (t-t_0)},
 * \quad t\geq t_0.
 * \f]
 *
 * @param dSpot (\f$S_0\f$) The spot price of the stock.
 * @param rTimes The maturities of forward contracts.
 * @param rForwards The market forward prices.
 * @param dLambda1 (\f$\lambda_1\geq 0\f$) The mean-reversion rate.
 * @param dSigma0 (\f$\sigma_0>0\f$) The volatility of the first factor.
 * @param dSigma1 (\f$\sigma_1>0\f$) The volatility of the second factor.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for forward prices.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted forward curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function forwardBlack2Fit (double dSpot,
                                const std::vector<double> &rTimes,
                                const std::vector<double> &rForwards,
                                double dLambda1, double dSigma0,
                                double dSigma1, double dInitialTime,
                                cfl::Function &rErr, cfl::FitParam &rParam);

/**
 * Returns discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * by the least-squares fit with method \p rFit of the yield curve
 * \f$\gamma = \gamma(t)\f$ to the market yields to maturity
 * determined by the swap rates.
 *
 * @param rSwapRates \f$(R_i)\f$ the vector of swap
 * rates, where \f$R_i\f$ is the market swap rate in the contract with
 * time interval \f$\delta t\f$ and number of payments \f$i\f$.
 * @param dPeriod (\f$\delta t\f$) The time interval between two payments.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for discount factors.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function discountSwapFit (const std::vector<double> &rSwapRates,
                               double dPeriod, double dInitialTime,
                               cfl::Fit &rFit, cfl::Function &rErr);
/** @} */
}

#endif // of __Review1_hpp__
