#ifndef __Examples_hpp__
#define __Examples_hpp__

/**
 * @file Examples.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Examples for the course.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"
#include "cfl/Fit.hpp"
#include "cfl/InterestRateModel.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Root.hpp"
#include "cfl/RootD.hpp"

/**
 * @mainpage Examples for the course "Financial Computing with C++"
 */

namespace prb
{
/**
 * @defgroup prbData Data structures.
 *
 * This module constructs input data curves for financial models.
 * @{
 */

/**
 * Computes the continuously compounded yield curve from a discount
 * curve. We recall that
 * \f[
 *  D(t) = e^{-\gamma(t)(t-t_0)}, \quad t\geq t_0.
 * \f]
 *
 * @param rDiscount (\f$(D(t))_{t\geq t_0}\f$) The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return \f$(\gamma(t))_{t\geq t_0}:\f$ the continuously compounded
 * yield curve.
 */
cfl::Function yield (const cfl::Function &rDiscount, double dInitialTime);

/**
 * Computes a part of yield curve for a number of interest rate
 * models (Hull-White, Nelson-Siegel, Svensson, Vasicek):
 * \f[
 * \Gamma(t) = \frac{1 - \exp(-\lambda (t-t_0))}{\lambda (t-t_0)},
 * \quad t\geq t_0.
 * \f]
 *
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return A part of yield curve for a number of interest rate
 * models.
 */
cfl::Function yieldShape1 (double dLambda, double dInitialTime);

/**
 * Computes a part of yield curve for a number of interest rate
 * models (Nelson-Siegel, Svensson):
 * \f[
 * \Gamma(t) = \frac{1 - \exp(-\lambda (t-t_0))}{\lambda (t-t_0)}
 * - \exp(-\lambda (t-t_0)), \quad t\geq t_0.
 * \f]
 *
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion
 * parameter.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return A part of yield curve for a number of interest rate
 * models.
 */
cfl::Function yieldShape2 (double dLambda, double dInitialTime);

/**
 * Computes the forward exchange rate curve. The exchange rate is
 * the number of units of domestic currency per one unit of foreign
 * currency.
 *
 * @param dSpotFX The spot exchange rate.
 * @param rDomesticDiscount The domestic discount curve.
 * @param rForeignDiscount The foreign discount curve.
 *
 * @return The forward exchange rate curve.
 */
cfl::Function forwardFX (double dSpotFX,
                         const cfl::Function &rDomesticDiscount,
                         const cfl::Function &rForeignDiscount);

/**
 * Computes the forward exchange rate curve. The exchange rate is
 * the number of units of domestic currency per one unit of foreign
 * currency.
 *
 * @param dSpotFX The spot exchange rate.
 * @param rDomesticDiscount The domestic discount curve.
 * @param rForeignDiscount The foreign discount curve.
 *
 * @return The forward exchange rate curve.
 */
cfl::Function forwardFX (double dSpotFX,
                         const cfl::Function &rDomesticDiscount,
                         const cfl::Function &rForeignDiscount);

/**
 * Computes the curve of forward prices ("clean" or "dirty") for a
 * coupon bond. The bond pays coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
 *     t_M = T.
 * \f]
 * The bond also pays notional \f$N=1\f$ at maturity \f$T\f$. The buyer of
 * the forward contract pays forward price \f$F(t)\f$ at delivery time
 * \f$t\f$ and then receives coupons \f$q\delta t\f$ at payments times
 * \f$t_i>t\f$ and notional \f$N=1\f$ at maturity \f$T\f$.
 *
 * @param dRate  (\f$q\f$) The coupon rate.
 * @param dPeriod  (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity  (\f$T\f$) The maturity.
 * @param rDiscount The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param bClean This parameter controls the output. If its value
 * is \p true, then we compute "clean" prices. Otherwise, we
 * compute "dirty" prices that include the accrued interest from
 * the next coupon.
 *
 * @return The forward price ("clean" or "dirty") curve for the coupon
 * bond.
 */
cfl::Function forwardCouponBond (double dRate, double dPeriod,
                                 double dMaturity,
                                 const cfl::Function &rDiscount,
                                 double dInitialTime, bool bClean);

/**
 * Computes the price of call option in the Black model.
 *
 * @param dK The strike.
 * @param dT The time to maturity.
 * @param dDF The discount factor.
 * @param dF The forward price.
 * @param dSigma The volatility.
 *
 * @return The price of the call in the Black model.
 */
double callBlack (double dK, double dT, double dDF, double dF, double dSigma);

/**
 * Computes the vega of call option in the Black model.
 *
 * @param dK The strike.
 * @param dT The time to maturity.
 * @param dDF The discount factor.
 * @param dF The forward price.
 * @param dSigma The volatility.
 *
 * @return The vega of the call in the Black model.
 */
double callVegaBlack (double dK, double dT, double dDF, double dF,
                      double dSigma);
/** @} */

/**
 * @defgroup examplesRoot Root finding.
 *
 * This module deals with algorithms for one-dimensional root
 * finding.
 *
 * @{
 */

/**
 * Computes the implied volatility of call option in the Black model using
 * Newton method.
 *
 * @param dK The strike.
 * @param dT The time to maturity.
 * @param dDF The discount factor.
 * @param dF The forward price.
 * @param dC The price of the call.
 * @param dSigma0 The initial guess for the volatility.
 * @param dErr The absolute error for the price.
 * @return The implied volatility of the call in the Black model.
 */
double callImplVolBlack (double dK, double dT, double dDF, double dF,
                         double dC, double dSigma0, double dErr);

/**
 * Computes the implied volatility of call option in the Black model.
 *
 * @param dK The strike.
 * @param dT The time to maturity.
 * @param dDF The discount factor.
 * @param dF The forward price.
 * @param dC The price of the call.
 * @param dSigma0 The initial guess for the volatility.
 * @param rRootD The one-dimensional root finder that relies
 * on the function and its derivative.
 *
 * @return The implied volatility of the call in the Black model.
 */
double callImplVolBlack (double dK, double dT, double dDF, double dF,
                         double dC, double dSigma0, const cfl::RootD &rRootD);

/**
 * Computes the implied volatility of the portfolio of calls, that is, the
 * value of volatility when Black and Scholes formula yields the market value.
 *
 * @param rCalls The portfolio of calls.
 * @param dPrice The market price of the portfolio.
 * @param rDiscount The discount curve.
 * @param rForward The forward curve.
 * @param dInitialTime The initial time.
 * @param dSigma0 The lower bound for volatility.
 * @param dSigma1 The upper bound for volatility.
 * @param rRoot The root finding method of bracketing type.
 * @return The implied volatility of the portfolio of calls.
 */
double callsImplVol (const std::vector<cfl::Data::Option> &rCalls,
                     double dPrice, const cfl::Function &rDiscount,
                     const cfl::Function &rForward, double dInitialTime,
                     double dSigma0, double dSigma1, const cfl::Root &rRoot);
/** @} */

/**
 * @defgroup prbInterp Interpolation of data curves.
 *
 * This module constructs input data curves for financial
 * models using interpolation.
 * @{
 */

/**
 * Computes the discount curve by the log-linear interpolation of a
 * given collection of market discount factors.
 *
 * @param rDiscountTimes The maturities of the market discount
 * factors.
 * @param rDiscountFactors The market discount factors.
 * @param dInitialTime The initial time.
 *
 * @return The discount curve obtained from the market discount
 * factors by the log-linear interpolation.
 */
cfl::Function
discountLogLinInterp (const std::vector<double> &rDiscountTimes,
                      const std::vector<double> &rDiscountFactors,
                      double dInitialTime);

/**
 * Computes forward curve
 * \f[
 *   F(t) = S_0 e^{q(t)(t-t_0)}, \quad t\in [t_0,t_M],
 * \f]
 * where the cost-of-carry rate \f$q=q(t)\f$ is obtained by the
 * interpolation of the market cost-of-carry rates with method \p
 * rInterp.
 *
 * @param dSpot (\f$S_0\f$) The spot price of the stock.
 * @param rDeliveryTimes (\f$(t_i)_{i=1,\dots,M}\f$) The delivery
 * times for the market forward prices.
 * @param rForwardPrices The market forward prices.
 * @param dInitialCarryRate (\f$q(t_0)\f$) The initial cost-of-carry rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rInterp The interpolation method for the cost-of-carry rates.
 *
 * @return The forward curve obtained by interpolation of
 * the market cost-of-carry rates with method \p rInterp.
 */
cfl::Function forwardCarryInterp (double dSpot,
                                  const std::vector<double> &rDeliveryTimes,
                                  const std::vector<double> &rForwardPrices,
                                  double dInitialCarryRate,
                                  double dInitialTime, cfl::Interp &rInterp);

/**
 * Computes forward curve
 * \f[
 * F(t) = S_0 e^{q(t)(t-t_0)}, \quad t\in [t_0,t_M],
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ is obtained by the Steffen
 * (monotone cubic spline) interpolation of the market cost-of-carry
 * rates. On the first interval \f$[t_0,t_1]\f$, the cost-of-carry
 * rate is constant:
 * \f[
 * q(t) = q(t_1), \quad t\in [t_0,t_1].
 * \f]
 *
 * @param dSpot (\f$S_0\f$) The spot price of the stock.
 * @param rDeliveryTimes (\f$(t_i)_{i=1,\dots,M}\f$) The maturities
 * of the market forward contracts, \f$t_0<t_1\f$.
 * @param rForwardPrices (\f$(F(t_i))_{i=1,\dots,M}\f$) The market
 * forward prices.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The forward curve obtained by the Steffen interpolation of
 * the market cost-of-carry rates.
 */
cfl::Function forwardCarrySteffenInterp (
    double dSpot, const std::vector<double> &rDeliveryTimes,
    const std::vector<double> &rForwardPrices, double dInitialTime);
/** @} */

/**
 * @defgroup prbFit Least-squares fitting of data curves.
 *
 * This module constructs input data curves for financial
 * models using least-squares fitting.
 * @{
 */

/**
 * Returns  discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * by the least-squares fit of yield curve \f$\gamma=\gamma(t)\f$ to
 * continuously compounded market yields with fitting method \p
 * rFit.
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dInitialTime  (\f$t_0\f$) The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for discount factors.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function discountYieldFit (const std::vector<double> &rTimes,
                                const std::vector<double> &rDF,
                                double dInitialTime, cfl::Fit &rFit,
                                cfl::Function &rErr);

/**
 * Returns discount curve
 * \f[
 *    d(t) = \exp(-r(t-t_0)), \quad t\geq t_0,
 * \f]
 * by the least-squares fit of constant interest rate \f$r\f$ to
 * continuously compounded market yields.
 *
 * @param rDiscountTimes The maturities for market discount factors.
 * @param rDiscountFactors The  market discount factors.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for discount factors.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted yield and its variance.
 */
cfl::Function
discountConstYieldFit (const std::vector<double> &rDiscountTimes,
                       const std::vector<double> &rDiscountFactors,
                       double dInitialTime, cfl::Function &rErr,
                       cfl::FitParam &rParam);

/**
 * Returns the discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * by the least-squares fit of Nelson-Siegel yield curve
 * \f[
 *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)} +
 *  c_2 \left(\frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)}  -
 *     e^{-\lambda (t-t_0)}\right), \quad t\geq t_0,
 * \f]
 * to continuously compounded market yields. Here \f$c_0\f$,
 * \f$c_1\f$, and \f$c_2\f$ are the fitting constants.
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for discount factors.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function discountNelsonSiegelFit (const std::vector<double> &rTimes,
                                       const std::vector<double> &rDF,
                                       double dLambda, double dInitialTime,
                                       cfl::Function &rErr,
                                       cfl::FitParam &rParam);

/**
 * Returns discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where yield curve \f$\gamma=\gamma(t)\f$ fits the market yields. In the
 * Vasicek model, \f[ \gamma(t; r_0, \theta ) = r_0 A(t) +
 * \frac{\theta}{\lambda}(1-A(t)) - \frac{\sigma^2}{2\lambda^2} (1-2A(t)+B(t)),
 * \quad t\geq t_0,
 * \f]
 * where
 * \f[
 * A(t) = \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)}, \quad B(t)
 * = \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)}.
 * \f]
 * The initial short-term rate \f$r_0\f$ and the drift \f$\theta\f$ are
 * obtained as the result of the least-squares fit of the market yields: \f[
 * \chi^2 = \min_{r_0,\theta} \sum_{i=1}^M (\gamma(t_i; r_0, \theta) -
 * \gamma_i)^2,  \quad \gamma_i = -\frac{\log d_i}{t_i-t_0}, \quad i=1,\dots,M.
 * \f]
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dSigma (\f$\sigma>0\f$) The volatility of short rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for discount factors.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function discountVasicekFit (const std::vector<double> &rTimes,
                                  const std::vector<double> &rDF,
                                  double dLambda, double dSigma,
                                  double dInitialTime, cfl::Function &rErr,
                                  cfl::FitParam &rParam);
/** @} */

/**
 * @defgroup cflAssetOptions Options on a single stock.
 *
 * This module deals with valuation of options on a single stock.
 * @{
 */

/**
 * Computes the value of the <strong>European put option</strong>.
 * In this contract, at maturity \p dMaturity a holder of the option
 * can sell the stock at strike \p dStrike.
 *
 * @param dStrike The strike of the option.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction put (double dStrike, double dMaturity,
                        cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>American put option</strong>.
 * In this contract, at any exercise time (from \p rExerciseTimes) a
 * holder of the option can sell the stock at strike \p dStrike.
 *
 * @param dStrike The strike of the option.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction americanPut (double dStrike,
                                const std::vector<double> &rExerciseTimes,
                                cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>up-or-down-and-out barrier
 * option</strong>. The payoff of the option at maturity (last
 * barrier time) is notional amount \p dNotional if the stock price
 * has not crossed the lower and upper barriers for all barrier
 * times. Otherwise, the option expires worthless.
 *
 * @param dLowerBarrier The lower barrier.
 * @param dUpperBarrier The upper barrier.
 * @param dNotional The notional amount.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time. The last barrier time is
 * the maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction barrierUpDownOut (double dNotional, double dLowerBarrier,
                                     double dUpperBarrier,
                                     const std::vector<double> &rBarrierTimes,
                                     cfl::AssetModel &rModel);

/**
 * At maturity \f$t_M\f$ the holder of the long position in the forward
 * contract receives the payment
 * \f[
 * V(t_M) = \frac1{M} \sum_{m=1}^{M} S(t_m) - G(t_0,t_M),
 * \f]
 * where \f$S(t)\f$ is the spot price at time \f$t\f$ and
 * \f$G(t_0,t_M)\f$ is the forward price computed at the initial time
 * \f$t_0\f$ for maturity \f$t_M\f$. The function computes the value of
 * the option in the  general case of <em>stochastic</em> interest rates.
 *
 * @param rAverTimes \f$(t_m)_{m=1,\dots,M}\f$ The averaging times.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction forwardOnAverageSpot (const std::vector<double> &rAverTimes,
                                         cfl::AssetModel &rModel);

/**
 * At maturity \f$t_M\f$ the holder of the long position in the forward
 * contract receives the payment
 * \f[
 * V(t_M) = \frac1{M} \sum_{m=1}^{M} S(t_m) - G(t_0,t_M),
 * \f]
 * where \f$S(t)\f$ is the spot price at time \f$t\f$ and
 * \f$G(t_0,t_M)\f$ is the forward price computed at the initial time
 * \f$t_0\f$ for maturity \f$t_M\f$. The function computes the value of
 * the option in the case of <em>deterministic</em> interest rates.
 *
 * @param rAverTimes \f$(t_m)_{m=1,\dots,M}\f$ The averaging times.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
forwardOnAverageSpotDetermRates (const std::vector<double> &rAverTimes,
                                 cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>down-and-out American
 * call</strong>.  The option behaves as the American call option
 * with strike \p dStrike and exercise times \p rExerciseTimes until
 * the first barrier time when the stock price hits lower barrier \p
 * dBarrier. At this exit time the option is canceled.
 *
 * @param dBarrier The lower barrier (\p dBarrier < \p dStrike).
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time.
 * @param dStrike The strike of the option.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
downOutAmericanCall (double dBarrier, const std::vector<double> &rBarrierTimes,
                     double dStrike, const std::vector<double> &rExerciseTimes,
                     cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>swing option</strong>.  The
 * holder can exercise the option \p iNumberOfExercises times.  At
 * each exercise time he can buy only one stock for strike \p
 * dStrike.
 *
 * @param dStrike The strike of the option.
 * @param rExerciseTimes The vector of exercise times.
 * @param iNumberOfExercises The maximal number of exercises.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */

cfl::MultiFunction swing (double dStrike,
                          const std::vector<double> &rExerciseTimes,
                          unsigned iNumberOfExercises,
                          cfl::AssetModel &rModel);

/**
 * Computes the price of <strong>cross-currency cap</strong>.
 * The payment times of the cap are given by
 * \f[
 *  t_m = t_0 + m \delta t, \quad m=1,\dots,M,
 * \f]
 * where \f$t_0\f$ is the initial time.  At payment time
 * \f$t_{m}\f$, a holder receives
 * \f[
 *  N\delta t\max(L^{dom}(t_{m-1},t_m) - L^{for}(t_{m-1},t_m) - \delta R,0),
 * \f]
 * where \f$L^{dom}(s,t)\f$ and \f$L^{for}(s,t)\f$ are the float
 * rates computed at time \f$s\f$ for maturity \f$t\f$ for domestic
 * and foreign currencies, respectively.
 *
 * @param rCap The parameters of the cross currency cap.
 * - \p rCap.rate \f$(\delta R)\f$ is the spread.
 * - \p rCap.notional \f$(N)\f$ is the notional.
 * - \p rCap.period \f$(\delta t)\f$ is the time interval between
 *   the payments.
 * - \p rCap.numberOfPayments \f$(M)\f$ is the number of payments.
 * @param rModel The reference to the implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction fxCrossCurrencyCap (const cfl::Data::CashFlow &rCap,
                                       cfl::AssetModel &rModel);
/** @} */

/**
 * @defgroup cflInterestRateOptions Options on interest rates.
 *
 * This module deals with valuation of standard and barrier
 * options on interest rates.
 *
 * @{
 */

/**
 * Computes the price of <strong>interest rate cap</strong>.
 *
 * @param rCap The parameters of interest rate cap.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction cap (const cfl::Data::CashFlow &rCap,
                        cfl::InterestRateModel &rModel);

/**
 * Computes the present value of <strong>interest rate
 * swap</strong>. The computations are done at issue time of the
 * swap contract.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction swap (const cfl::Data::Swap &rSwap,
                         cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>interest rate swaption</strong>.
 * In this contract, at maturity \p dMaturity a holder of the option
 * has the right to enter the interest rate swap with issue time \p
 * dMaturity and parameters \p rSwap.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param dMaturity The maturity of the option. This time is also
 * the issue time for the underlying swap contract.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction swaption (const cfl::Data::Swap &rSwap, double dMaturity,
                             cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>cancellable collar</strong>.
 * In this contract, holder of the option can terminate the
 * underlying interest rate collar at any payment time.
 * Note that in the case of termination the current payment
 * is still taking place.
 *
 * @param rCap The parameters of the underlying interest rate
 * collar. Here <code>rCap.rate</code> defines the cap rate.
 * @param dFloorRate The floor rate in the underlying interest
 * rate collar.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction cancellableCollar (const cfl::Data::CashFlow &rCap,
                                      double dFloorRate,
                                      cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>down-and-out cap</strong>.  The
 * down-and-out cap generates the same cash flow as a standard
 * interest rate cap up to and including the payment time when the
 * float rate hits lower barrier \p dLowerBarrier. After that, the
 * cap is terminated.
 *
 * @param rCap The parameters of the underlying interest rate
 * cap.
 * @param dLowerBarrier The lower barrier for float rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction downOutCap (const cfl::Data::CashFlow &rCap,
                               double dLowerBarrier,
                               cfl::InterestRateModel &rModel);

/**
 * Computes  <strong>futures price on float rate</strong>.
 * The set of times, when the futures price is evaluated is given by
 * \f[
 * t_i = t_0 + i\frac{T-t_0}{M}, \quad i=1,\dots,M,
 * \f]
 * where \f$t_0=\f$ \p rModel.initialTime(), \f$T=\f$ \p
 * dFuturesMaturity, and \f$M=\f$ \p iFuturesTimes.
 * At maturity \p dFuturesMaturity, the settlement price is
 * \f[
 * 1 - L(T,T+\Delta),
 * \f]
 * where \f$\Delta = \f$ \p dRatePeriod and \f$L(s,t)\f$ is the
 * float rate computed at \f$s\f$ for maturity \f$t\f$.
 *
 * @param dRatePeriod The rate period in the futures.
 * @param iFuturesTimes The number of futures times.
 * @param dMaturity The maturity.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction futuresOnRate (double dRatePeriod, unsigned iFuturesTimes,
                                  double dMaturity,
                                  cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>drop-lock swap</strong>.  The contract
 * behaves as a standard swap with the difference that the first payment
 * time the market swap rate is above \p dUpperRate or below \p dLowerRate
 * the fixed rate in the contract is reset to \p dUpperRate or \p
 * dLowerRate, respectively, and then remains constant. The fixed rate
 * before the reset time equals \p rSwap.rate. The market swap rate used for
 * resets corresponds to the swap contract with the same parameters as in
 * the original \p rSwap, that is, the same number of payments and the
 * period. Note that the initial time is <em>not</em> a reset time.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param dLowerRate The lower value for swap rate after reset.
 * @param dUpperRate The upper bound for swap rate after reset.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction dropLockSwap (const cfl::Data::Swap &rSwap,
                                 double dLowerRate, double dUpperRate,
                                 cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>auto interest rate cap</strong>.  The
 * holder receives first \p iNumberOfCaplets in-the-money caplets and then
 * the contract is terminated.
 *
 * @param rCap The parameters of interest rate cap.
 * @param iNumberOfCaplets The maximal number of paid caplets. It is
 * less than or equal to \p rCap.numberOfPayments.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction autoCap (const cfl::Data::CashFlow &rCap,
                            unsigned iNumberOfCaplets,
                            cfl::InterestRateModel &rModel);
/** @} */
}

#endif // of __Examples_hpp__
