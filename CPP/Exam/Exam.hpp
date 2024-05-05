#ifndef __Exam_hpp__
#define __Exam_hpp__

/**
 * @file Exam.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Exam for the course.
 * @version 1.0
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"
#include "cfl/Fit.hpp"
#include "cfl/InterestRateModel.hpp"
#include "cfl/Interp.hpp"

/**
 * @mainpage Exam for the course "Financial Computing with C++"
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
 * Computes the curve of forward swap rates.
 *
 * @param dPeriod The time interval between the payments in the swap.
 * @param iNumberOfPayments The number of payments in the swap.
 * @param rDiscount The discount curve.
 * @return cfl::Function The curve of forward swap rates.
 */
cfl::Function forwardSwapRate (double dPeriod, unsigned iNumberOfPayments,
                               const cfl::Function &rDiscount);

/** @} */

/**
 * @defgroup  prbInterp Interpolation.
 *
 * This module deals with numerical interpolation of input data curves.
 * @{
 */

/**
 * Computes the discount curve from a family of swap rates.  The
 * computation is done in two steps:
 * 1. We deduce the discount factors for the maturities of swap contracts.
 * 2. We apply log-linear interpolation to these discount factors.
 *
 * @param rSwapRates (\f$(R_i)_{i=1,\dots,M}\f$) The market swap
 * rates. The maturities of the contracts have the form:
 * \f[
 * t_i = t_0 + i\delta t, \quad i=1,\dots,M.
 * \f]
 * @param dPeriod (\f$\delta t\f$) The time interval between two
 * payments in a swap.
 * @param dInitialTime (\f$t_0\f$) The initial time.
 *
 * @return The discount curve obtained from the market swap rates by
 * log-linear interpolation of discount factors.
 */
cfl::Function discountSwapLogLinInterp (const std::vector<double> &rSwapRates,
                                        double dPeriod, double dInitialTime);

/** @} */

/**
 * @defgroup cflAssetOptions Options on a single stock.
 *
 * This module deals with valuation of options on a single stock.
 * @{
 */

/**
 * Computes the value of the <strong>down-and-rebate or up-and-out barrier
 * option</strong>. There are two types of barrier events:
 * <ol>
 * <li> <em>The lower barrier event</em> is taking place at the first time
 * when the price of the stock is below the lower barrier
 * \p dLowerBarrier. In this case, the option pays the notional amount
 * \p dNotional.
 * <li> <em>The upper barrier event</em> is taking place at the
 * first time when the price of the stock is above the upper barrier
 * \p dUpperBarrier. In this case, the option expires worthless.
 * </ol>
 * After any of the barrier events the option is terminated. If neither of
 * the barriers has been crossed, then the option expires worthless.
 *
 * @param dNotional The notional amount.
 * @param dLowerBarrier The lower barrier.
 * @param dUpperBarrier The upper barrier.
 * @param rLowerBarrierTimes The lower barrier times.
 * @param rUpperBarrierTimes The upper barrier times.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
downRebateUpOut (double dNotional, double dLowerBarrier, double dUpperBarrier,
                 const std::vector<double> &rLowerBarrierTimes,
                 const std::vector<double> &rUpperBarrierTimes,
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
 * Computes the price of <strong>floor with deferred
 * payments</strong>.  In this contract, all floor payments are
 * made at maturity (the last payment time).
 *
 * @param rFloor The parameters of interest rate floor.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
floorWithDeferredPayments (const cfl::Data::CashFlow &rFloor,
                           cfl::InterestRateModel &rModel);

/** @} */
} // namespace prb

#endif // of __Exam_hpp__
