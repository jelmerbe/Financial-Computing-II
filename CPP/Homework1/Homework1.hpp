#ifndef __Homework1_hpp__
#define __Homework1_hpp__

/**
 * @file Homework1.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Homework 1: data curves for financial models
 * @version 0.1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"

/**
 * @mainpage Homework 1: data curves for financial models
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
 * Computes volatility curve \f$\Sigma = \Sigma(t)\f$ from
 * variance curve \f$V = V(t)\f$:
 * \f[
 * V(t) = \Sigma^2(t)(t-t_0), \quad t\geq t_0.
 * \f]
 *
 * @param rVar (\f$V(t)\f$) The variance function.
 * @param dInitialTime  (\f$t_0\f$) The initial time.
 * @return cfl::Function The volatility curve.
 */
cfl::Function volatilityVar (const cfl::Function &rVar, double dInitialTime);

/**
 * Computes cost-of-carry rate curve \f$c=(c(t))_{t\geq t_0}\f$
 * for the Black model:
 * \f[
 * q(t) = \theta \frac{1-e^{-\lambda(t-t_0)}}{\lambda(t-t_0)} +
 * \frac{\sigma^2}2 \frac{1-e^{-2\lambda(t-t_0)}}{2\lambda(t-t_0)},
 * \quad t\geq t_0.
 * \f]
 *
 * @param dTheta (\f$\theta\f$) The drift term.
 * @param dLambda (\f$\lambda\geq 0\f$) The mean reversion level.
 * @param dSigma  (\f$\sigma\geq 0\f$) The volatility.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The cost-of-carry rate curve in Black model.
 */
cfl::Function carryBlack (double dTheta, double dLambda, double dSigma,
                          double dInitialTime);

/**
 * Computes the swap rate.
 *
 * @param dPeriod The period in the swap.
 * @param iPayments The number of payments in the swap.
 * @param rDiscount The discount curve.
 * @param dInitialTime The initial time (=issue time for the swap).
 * @return The fixed rate in the swap.
 */
double swapRate (double dPeriod, unsigned iPayments,
                 const cfl::Function &rDiscount, double dInitialTime);

/**
 * Computes the curve of forward prices for a cash flow.  The buyer
 * pays forward price \f$F(t)\f$ at delivery time \f$t\f$ and then
 * receives payments \f$(P_i)\f$ at payments times \f$t_i>t\f$.
 *
 * @param rPayments (\f$(P_i)\f$) The vector of payments.
 * @param rPaymentTimes (\f$(t_i)\f$) The vector of payment times.
 * The first payment time has to be strictly greater than the
 * initial time.
 *
 * @param rDiscount The discount curve.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The forward prices \f$(F(t))_{t\in [t_0,T]}\f$ for the cash
 * flow, where \f$T\f$ is the last payment time.
 */
cfl::Function forwardCashFlow (const std::vector<double> &rPayments,
                               const std::vector<double> &rPaymentTimes,
                               const cfl::Function &rDiscount,
                               double dInitialTime);

/** @} */
}

#endif // of __Homework1_hpp__
