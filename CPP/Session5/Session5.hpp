#ifndef __Session5_hpp__
#define __Session5_hpp__

/**
 * @file Session5.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Session 5: standard and barrier options on interest rates.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/InterestRateModel.hpp"

/**
 * @mainpage Session 5: standard and barrier options on interest rates.
 */

namespace prb
{
/**
 * @defgroup cflInterestRateOptions Options on interest rates.
 *
 * This module deals with valuation of standard and barrier
 * options on interest rates.
 *
 * @{
 */

/**
 * Computes the price of <strong>interest rate floor</strong>.
 *
 * @param rFloor The parameters of the interest rate floor.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction floor (const cfl::Data::CashFlow &rFloor,
                          cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>put option on forward rate
 * agreement</strong>. In this contract, at maturity \p dMaturity
 * the holder of the option has the right to borrow notional amount
 * \p dNotional at interest rate \p dFixedRate for period \p
 * dPeriod.
 *
 * @param dFixedRate The fixed interest rate.
 * @param dPeriod The period of interest rate loan.
 * @param dNotional The notional amount.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction putOnFRA (double dFixedRate, double dPeriod,
                             double dNotional, double dMaturity,
                             cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>cap on swap rate</strong>.
 * In this contract, the payments occur at times
 * \f[
 * t_i = t_0 + i \delta t, \quad i=1, \dots, M,
 * \f]
 * where \f$t_0=\f$ \p rModel.initialTime(), \f$\delta t=\f$ \p
 * rCap.period, and \f$M=\f$ \p rCap.numberOfPayments. The payment
 * at \f$t_i\f$ has the form:
 * \f[
 * N\delta t \max(R(t_{i-1}) - C,0),
 * \f]
 * where \f$N=\f$ \p rCap.notional and \f$R(t_{i-1})\f$ is the swap
 * rate computed at <em>previous</em> time \f$t_{i-1}\f$ in the swap
 * contract with number of payments \p iSwapPayments and the
 * interval between the payments \p dSwapPeriod.
 *
 * @param rCap The parameters of interest rate cap.
 * @param dSwapPeriod The period in the interest rate swap.
 * @param iSwapPayments The number of payments in the interest rate swap.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction capOnSwapRate (const cfl::Data::CashFlow &rCap,
                                  double dSwapPeriod, unsigned iSwapPayments,
                                  cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>cancellable swap set and paid in
 * arrears</strong>.  In this contract, the holder of the option has
 * the right to cancel the swap at any payment time (after the
 * payments). The fixed payments are as in the usual swap. However,
 * the float payments are set in <em>arrears</em>. This means that
 * the float payment at a given payment time is determined by the
 * rate computed at <em>this</em> payment time (in the case of
 * standard swap the float payment is set at the <em>previous</em>
 * payment time).
 *
 * @param rSwap The parameters of the swap.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction cancelSwapArrears (const cfl::Data::Swap &rSwap,
                                      cfl::InterestRateModel &rModel);

/** @} */
} // namespace prb

#endif // of __Session5_hpp__
