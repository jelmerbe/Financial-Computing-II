#ifndef __ReviewExam_hpp__
#define __ReviewExam_hpp__

/**
 * @file ReviewExam.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief ReviewExam for the course.
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"
#include "cfl/InterestRateModel.hpp"
#include "cfl/Interp.hpp"

/**
 * @mainpage ReviewExam for the course "Financial Computing with C++"
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
 * Computes the (dollar) duration of an annuity. The annuity pays
 * coupons \f$q\delta t\f$ at times
 * \f$(t_i)_{i=1,\dots,M}\f$ such that
 * \f[
 *     t_0 < t_1 \leq t_0+\delta t, \quad t_{i+1}-t_i= \delta t, \quad
 *     t_M = T.
 * \f]
 * The annuity also pays notional \f$N=1\f$ at maturity \f$T\f$. The
 * duration for the annuity at \f$t_0\f$ defined as
 * \f[
 *  D = - P'(\gamma) = - \frac{dP}{d\gamma},
 * \f]
 * where \f$P = P(\gamma)\f$ is the ``dirty'' price of the annuity for YTM
 * \f$\gamma\f$.
 *
 * @param dRate  (\f$q\f$) The coupon rate.
 * @param dPeriod  (\f$\delta t\f$) The time interval between payments.
 * @param dMaturity  (\f$T\f$) The maturity.
 * @param dYTM (\f$\gamma\f$) The continuously compounded yield to maturity.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The cash duration for the annuity.
 */
double durationAnnuity (double dRate, double dPeriod, double dMaturity,
                        double dYTM, double dInitialTime);

/** @} */

/**
 * @defgroup prbInterp Interpolation of data curves.
 *
 * This module constructs input data curves for financial
 * models using interpolation.
 * @{
 */

/**
 * Computes the discount curve from a family of float rates. The
 * computation is done in two steps:
 * 1. We obtain the discount factors for the maturities of float
 * rates.
 * 2. We apply log-linear interpolation to these discount factors.
 *
 * @param rRatePeriods The periods of float rates. For example, the
 * value 0.25 for a period means the 3 month float rate.
 *@param rRates The vector of market float rates.
 * @param dInitialTime The initial time.
 *
 * @return The discount curve obtained from the market float rates
 * by log-linear interpolation of discount factors.
 */
cfl::Function
discountRateLogLinInterp (const std::vector<double> &rRatePeriods,
                          const std::vector<double> &rRates,
                          double dInitialTime);
/** @} */

/**
 * @defgroup cflAssetOptions Options on a single stock.
 *
 * This module deals with valuation of options on a single stock.
 * @{
 */

/**
 * Computes the value of the <strong>swing with penalty paid at
 * maturity</strong>.  The holder can exercise the option \p
 * iNumberOfExercises times.  At each exercise time he can buy
 * only one stock for the strike \p dStrike. The cumulative sum of
 * strikes is paid at \p dMaturity. In addition, at maturity,
 * the holder has to pay the penalty amount \p dPenalty per each
 * non-exercised option.
 *
 * @param dStrike The strike of the option. It is paid at maturity.
 * @param rExerciseTimes The vector of exercise times.
 * @param iNumberOfExercises The maximal number of exercises.
 * @param dPenalty The penalty amount per every non-exercised option.
 * It is paid at maturity.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
swingWithPenaltyPaidAtMaturity (double dStrike,
                                const std::vector<double> &rExerciseTimes,
                                unsigned iNumberOfExercises, double dPenalty,
                                double dMaturity, cfl::AssetModel &rModel);
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
 * Computes the value of <strong>interest rate swap set in arrears</strong>.
 * The fixed payments are as in the usual swap. The float payments
 * are set in \em arrears: the current float payment is determined
 * by the <em>current</em> float rate (in the case of standard swap,
 * the float payment is set at the \em previous payment time).
 * We assume that today is the issue date of the swap and denote this
 * time by \f$t_0\f$.  The payment times of the swap are given by
 * \f[
 * t_m = t_0 + m \delta t, \quad m=1,\dots,M.
 * \f]
 * At time \f$t_m\f$, the side paying the float rate receives the amount
 * \f[
 * NR\delta t - N r(t_{m+1},t_{m+1}+\delta t)\delta t,
 * \f]
 * where \f$r(s,t)\f$ is the float rate computed at \f$s\f$ for maturity
 * \f$t\f$.
 *
 * @param rSwap The parameters of the swap: \f$N = \f$ \a rSwap.notional,
 * \f$\delta t = \f$ \a rSwap.period, \f$M=\f$ \a rSwap.numberOfPayments,
 * \f$R = \f$ \a rSwap.rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction swapArrears (const cfl::Data::Swap &rSwap,
                                cfl::InterestRateModel &rModel);

/** @} */
}

#endif // of __ReviewExam_hpp__
