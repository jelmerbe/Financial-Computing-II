#ifndef __Session2_hpp__
#define __Session2_hpp__

/**
 * @file Session2.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Session 2: root finding and interpolation
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/Interp.hpp"
#include "cfl/RootD.hpp"

/**
 * @mainpage Session 2: root finding and interpolation
 */

namespace prb
{
/**
 * @defgroup Session2Root Root finding.
 *
 * This module deals with algorithms for one-dimensional root
 * finding.
 *
 * @{
 */

/**
 * Computes prices ("clean" or "dirty") for a coupon bond. The bond
 * pays coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
 *     t_M = T.
 * \f]
 * The bond also pays notional \f$N=1\f$ at maturity \f$T\f$. The dirty or cash
 * price of the bond is given by
 * \f[
 * P^{d}(t_0) = q\delta t \sum_{i=1}^M e^{-\gamma(t_i-t_0)} +
 * e^{-\gamma(T-t_0)}. \f] The clean price has the form: \f[ P^{c}(t_0) =
 * P^d(t_0) - A(t_0) = P^d(t_0) - q(t_0-s), \f] where \f$A(t_0)\f$ is the
 * accrued interest and \f$s\f$ is the previous coupon time.
 *
 * @param dRate  \f$q\f$ The coupon rate.
 * @param dPeriod  \f$\delta t\f$ The time interval between payments.
 * @param dMaturity  \f$T\f$ The maturity.
 * @param dYTM \f$\gamma\f$ The continuously compounded yield to maturity.
 * @param dInitialTime \f$t_0\f$ The initial time.
 * @param bClean This parameter controls the output. If its value
 * is \p true, then we compute "clean" prices. Otherwise, we
 * compute "dirty" prices that include the accrued interest from
 * the next coupon.
 *
 * @return The price ("clean" or "dirty") for the coupon bond.
 */
double couponBond (double dRate, double dPeriod, double dMaturity, double dYTM,
                   double dInitialTime, bool bClean);

/**
* Computes the (dollar) duration of a coupon bond. The bond
* pays coupons \f$q\delta t\f$ at times
* \f$(t_i)_{i=1,\dots,M}\f$ such that
* \f[
*     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
*     t_M = T.
* \f]

* The bond also pays notional \f$N=1\f$ at maturity \f$T\f$. The
* duration for the bond at \f$t_0\f$ defined as
* \f[
*  D = - P'(\gamma) = - \frac{dP}{d\gamma},
* \f]
* where \f$P = P(\gamma)\f$ is the ``dirty'' price of the bond for YTM
* \f$\gamma\f$.
*
* @param dRate  (\f$q\f$) The coupon rate.
* @param dPeriod  (\f$\delta t\f$) The time interval between payments.
* @param dMaturity  (\f$T\f$) The maturity.
* @param dYTM (\f$\gamma\f$) The continuously compounded yield to maturity.
* @param dInitialTime (\f$t_0\f$) The initial time.
*
* @return The price ("clean" or "dirty") for the coupon bond.
*/
double durationCouponBond (double dRate, double dPeriod, double dMaturity,
                           double dYTM, double dInitialTime);

/**
 * Computes the continuously compounded YTM for a coupon bond. The
 * coupon bond pays coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *  t_0<t_1\leq t_0+\delta t, \quad t_{i+1}-t_i = \delta t,\quad
 *  t_M = T.
 * \f]
 * Yield to maturity \f$\gamma\f$ solves the equation:
 * \f[
 * S(t_0) = q\delta t \sum_{i=1}^M e^{-\gamma(t_i-t_0)} + e^{-\gamma(T-t_0)}.
 * \f]
 * We find \f$\gamma\f$ using Newton's gradient descent method.  If
 * \f$\gamma_n\f$ is yield's estimate at step \f$n\f$, then we stop
 * iterations as soon as
 * \f[
 *  |\gamma_{n+1}-\gamma_n|<\epsilon.
 * \f]
 *
 * @param dRate (\f$q\f$) The coupon rate.
 * @param dPeriod (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity (\f$T\f$) The maturity.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param dPrice (\f$S(t_0)\f$) The current price of the coupon bond.
 * @param dYield0 The initial guess of the YTM.
 * @param dErr The absolute error for the YTM.
 *
 * @return The YTM of the coupon bond.
 */
double yieldTMCouponBond (double dRate, double dPeriod, double dMaturity,
                          double dInitialTime, double dPrice, double dYield0,
                          double dErr);

/**
 * Computes the continuously compounded YTM for a coupon bond. The
 * coupon bond pays coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *  t_0<t_1\leq t_0+\delta t, \quad t_{i+1}-t_i = \delta t,\quad
 *  t_M = T.
 * \f]
 * Yield to maturity \f$\gamma\f$ solves the equation:
 * \f[
 * S(t_0) = q\delta t \sum_{i=1}^M e^{-\gamma(t_i-t_0)} + e^{-\gamma(T-t_0)}.
 * \f]
 *
 * @param dRate (\f$q\f$) The coupon rate.
 * @param dPeriod (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity (\f$T\f$) The maturity.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 * @param dPrice (\f$S(t_0)\f$) The current price of the coupon bond.
 * @param dYield0 The initial guess of the YTM.
 * @param rRootD The one-dimensional root finder that relies
 * on the function and its derivative.
 *
 * @return The YTM of the coupon bond.
 */
double yieldTMCouponBond (double dRate, double dPeriod, double dMaturity,
                          double dInitialTime, double dPrice, double dYield0,
                          const cfl::RootD &rRootD);

/** @} */

/**
 * @defgroup prbInterp Interpolation of data curves.
 *
 * This module constructs input data curves for financial
 * models using interpolation.
 * @{
 */

/**
 * Computes a discount curve by interpolation of market yields.  The
 * construction of the discount curve is accomplished in 3 steps:
 * 1. We obtain the market yields from discount factors \p rDF.
 * 2. We get the market yield curve by interpolation with \p rInterp.
 * 3. We compute the discount curve from the yield curve.
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dR0 The initial short-term interest rate.
 * @param dInitialTime The initial time.
 * @param rInterp The interpolation method for yields.
 *
 * @return cfl::Function The interpolated discount curve.
 */
cfl::Function discountYieldInterp (const std::vector<double> &rTimes,
                                   const std::vector<double> &rDF, double dR0,
                                   double dInitialTime, cfl::Interp &rInterp);

/**
 * Computes a discount curve by Steffen (monotone cubic spline)
 * interpolation of market yields.  The construction of the discount
 * curve is accomplished in 3 steps:
 * 1. We obtain the market yields from discount factors \p rDF.
 * 2. We get the market yield curve by Steffen interpolation.
 * 3. We compute the discount curve from the yield curve.
 *
 * @param rTimes The maturities of market discount factors.
 * @param rDF The market discount factors.
 * @param dR0 The initial short-term interest rate.
 * @param dInitialTime The initial time.
 *
 * @return cfl::Function The interpolated discount curve.
 */
cfl::Function discountYieldSteffenInterp (const std::vector<double> &rTimes,
                                          const std::vector<double> &rDF,
                                          double dR0, double dInitialTime);
/** @} */
}

#endif // of __Session2_hpp__
