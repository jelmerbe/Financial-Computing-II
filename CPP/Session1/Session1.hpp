#ifndef __Session1_hpp__
#define __Session1_hpp__

/**
 * @file Session1.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Session 1: data curves for financial models
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"

/**
 * @mainpage Session 1: data curves for financial models
 */

namespace prb
{
/**
 * @defgroup Session1Data Data structures.
 *
 * This module constructs input data curves for financial models.
 * @{
 */

/**
 * Returns the stationary implied volatility curve for the Black
 * model:
 * \f[
 * \Sigma(t) = \sigma \sqrt{\frac{1 -\exp(-2\lambda
 *   (t-t_0))}{2\lambda (t-t_0)}}, \quad t\geq t_0.
 * \f]
 *
 * @param dSigma (\f$\sigma\geq 0\f$) The short-term volatility.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean-reversion rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The stationary implied volatility curve in Black model.
 */
cfl::Function volatilityBlack (double dSigma, double dLambda,
                               double dInitialTime);

/**
 * Computes the Nelson-Siegel yield curve:
 * \f[
 *  \gamma(t) = c_0 + c_1 \frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)} +
 *  c_2 \left(\frac{1-e^{-\lambda(t-t_0)}}{\lambda (t-t_0)}  -
 *     e^{-\lambda (t-t_0)}\right), \quad t\geq t_0.
 * \f]
 *
 * @param dC0 (\f$c_0\f$) The first constant
 * @param dC1 (\f$c_1\f$) The second constant.
 * @param dC2 (\f$c_2\f$) The third constant.
 * @param dLambda (\f$\lambda>0\f$) The mean-reversion rate.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The yield curve for the Nelson-Siegel model.
 */
cfl::Function yieldNelsonSiegel (double dC0, double dC1, double dC2,
                                 double dLambda, double dInitialTime);

/**
 * Computes the curve of forward rates. It costs nothing to enter
 * the forward rate agreement (FRA) with maturity \f$t\f$ and period
 * \f$\delta t\f$ as either a borrower or a lender. Forward rate
 * \f$R^f(t,t+\delta t)\f$ is set at issue time \f$t_0\f$. In the
 * contract,
 * 1. At maturity \f$t\f$, the borrower receives notional \f$N\f$.
 * 2. At time \f$t+\delta t\f$, the borrower pays the notional plus
 * the interest computed at the forward rate:
 * \f[
 *    N(1+ R^f(t,t+\delta t)\delta t).
 * \f]
 *
 * @param dPeriod (\f$\delta t\f$)  The rate period.
 * @param rDiscount The discount curve.
 * @return cfl::Function \f$(R^f(t,t+\delta t))_{t\geq t_0}\f$ The
 * curve of forward rates.
 */
cfl::Function forwardRate (double dPeriod, const cfl::Function &rDiscount);

/**
 * Computes the curve of forward prices for a dividend paying
 * stock. The buyer pays forward price \f$F(t)\f$ at delivery time
 * \f$t\f$ and then receives the stock. If \f$t\f$ is a dividend
 * time, then the buyer gets the dividend paid at \f$t\f$. It costs
 * nothing to enter a forward contract at its issue time \f$t_0\f$.
 *
 * @param dSpot The spot price of the stock.
 * @param rDividendsTimes The dividend times.
 * @param rDividends The dividend payments.
 * @param rDiscount The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The forward price curve for the stock.
 */
cfl::Function
forwardStockDividends (double dSpot,
                       const std::vector<double> &rDividendsTimes,
                       const std::vector<double> &rDividends,
                       const cfl::Function &rDiscount, double dInitialTime);
/** @} */
} // namespace prb

#endif // of __Session1_hpp__
