#ifndef __Homework3_hpp__
#define __Homework3_hpp__

/**
 * @file Homework3.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Homework 3: least-squares fit of data curves
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
 * @mainpage Homework 3: least-squares fit of data curves.
 */

namespace prb
{

/**
 * @defgroup prbFit Least-squares fit of data curves.
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
 * by the least-squares fit of Svensson yield curve
 * \f[
 *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda_1(t-t_0)}}{\lambda_1 (t-t_0)} +
 *  c_2 \left(\frac{1-e^{-\lambda_1(t-t_0)}}{\lambda_1 (t-t_0)}  -
 *     e^{-\lambda_1 (t-t_0)}\right) +
 *  c_3 \left(\frac{1-e^{-\lambda_2(t-t_0)}}{\lambda_2 (t-t_0)}  -
 *     e^{-\lambda_2 (t-t_0)}\right) , \quad t\geq t_0,
 * \f]
 * to continuously compounded market yields. Here \f$c_0\f$,
 * \f$c_1\f$, \f$c_2\f$, and \f$c_3\f$ are the fitting constants.
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dLambda1 (\f$\lambda_1 \geq 0\f$) The first mean-reversion rate.
 * @param dLambda2 (\f$\lambda_2 \geq 0\f$) The second
 * mean-reversion rate, \f$\lambda_2\not=\lambda_1\f$.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for discount factors.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function discountSvenssonFit (const std::vector<double> &rTimes,
                                   const std::vector<double> &rDF,
                                   double dLambda1, double dLambda2,
                                   double dInitialTime, cfl::Function &rErr,
                                   cfl::FitParam &rParam);

/**
 * Returns  discount curve
 * \f[
 *    d(t) = \exp(-\gamma(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * by the least-squares fit with method \p rFit of yield curve
 * \f$\gamma=\gamma(t)\f$ to continuously compounded market yields
 * determined by float rates.
 *
 * @param rPeriods The periods of float rates. For example,
 * value 0.25 means 3 month rate.
 * @param rRates The  market float rates.
 * @param dInitialTime The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for discount factors.
 *
 * @return The fitted discount curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function discountRateFit (const std::vector<double> &rPeriods,
                               const std::vector<double> &rRates,
                               double dInitialTime, cfl::Fit &rFit,
                               cfl::Function &rErr);

/**
 * Returns the forward exchange rate curve in the form:
 * \f[
 * F(t) = S_0 \exp(q(t)(t-t_0)), \quad t\geq t_0,
 * \f]
 * where cost-of-carry rate \f$q=q(t)\f$ is obtained by the
 * least-squares fit of the market cost-of-carry rates with method
 * \p rFit.
 *
 * @param dSpotFX The spot exchange rate.
 * @param rTimes The  maturities for discount factors.
 * @param rDomesticDiscountFactors The  domestic discount factors.
 * @param rForeignDiscountFactors The  foreign discount factors.
 * @param dInitialTime The initial time.
 * @param rFit The fitting method.
 * @param rErr The error function of the fit for forward prices.
 *
 * @return The fitted forward fx curve and its error function. We
 * also obtain the fitted coefficients and their covariance matrix
 * through \p rFit.
 */
cfl::Function
forwardFXCarryFit (double dSpotFX, const std::vector<double> &rTimes,
                   const std::vector<double> &rDomesticDiscountFactors,
                   const std::vector<double> &rForeignDiscountFactors,
                   double dInitialTime, cfl::Fit &rFit, cfl::Function &rErr);

/**
 * Returns the stationary implied volatility curve for the Black model by
 * the least-squares approximation of the market implied volatilities.
 * The stationary volatility form for Black model is given by
 * \f[
 * \Sigma(t) = \sigma \sqrt{\frac{1 -\exp(-2\lambda
 *   (t-t_0))}{2\lambda (t-t_0)}}, \quad t\geq t_0,
 * \f]
 * where short-term volatility \f$ \sigma \f$ is the  fitted
 * parameter.
 *
 * @param rTimes The maturities of the market implied volatilities.
 * @param rVols The market implied volatilities.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param rErr The error function of the fit for implied volatilities.
 * @param rParam The fitted coefficients, their covariance matrix, and
 * the total \f$\chi^2\f$ fitting error.
 *
 * @return The fitted volatility curve and its error function. We also
 * obtain the fitted coefficients and their covariance matrix.
 */
cfl::Function volatilityBlackFit (const std::vector<double> &rTimes,
                                  const std::vector<double> &rVols,
                                  double dLambda, double dInitialTime,
                                  cfl::Function &rErr, cfl::FitParam &rParam);

/** @} */
}

#endif // of __Homework3_hpp__
