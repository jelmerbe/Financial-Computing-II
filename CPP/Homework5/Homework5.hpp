#ifndef __Homework5_hpp__
#define __Homework5_hpp__

/**
 * @file Homework5.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Homework 5: standard and barrier options on interest rates.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/InterestRateModel.hpp"

/**
 * @mainpage Homework 5: standard and barrier options on interest rates.
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
 * Computes the price of <strong>interest rate collar</strong>.
 *
 * @param rCap The parameters of the underlying interest rate
 * collar. Here \p rCap.rate defines the cap rate.
 * @param dFloorRate The floor rate in the underlying interest
 * rate collar.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction collar (const cfl::Data::CashFlow &rCap, double dFloorRate,
                           cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>American swaption</strong>.
 * A holder of the option can enter into the underlying swap agreement at
 * any exercise time. This time then becomes the issue time of the swap.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param rExerciseTimes The set of exercise times. The first
 * exercise time is strictly greater than the initial time in the
 * model.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction americanSwaption (const cfl::Data::Swap &rSwap,
                                     const std::vector<double> &rExerciseTimes,
                                     cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>puttable and callable coupon
 * bond</strong>.
 * In this contract, at any coupon time less than maturity the
 * following events take place in their respective order:
 * <ol>
 * <li> The coupon payment.
 * <li> Holder of the bond has the right to sell the bond to the
 * issuer. In this case, the holder receives the redemption amount
 * <code>rCouponBond.notional * dRedemptionPrice</code> and the bond is
 * terminated.
 * <li> Issuer of the bond has the right to buy the bond back. In this
 * case, he pays the repurchase amount <code>rCouponBond.notional *
 * dRepurchasePrice</code> and the bond is terminated.
 * </ol>
 *
 * @param rCouponBond The parameters of the underlying coupon
 * bond. The issue time for the bond coincides with the initial
 * time of the model.
 * @param dRedemptionPrice The redemption price of the bond given
 * as a percentage of the notional. After the coupon payment, the
 * holder of the bond can sell it back to the issuer for the
 * amount <code>rCouponBond.notional * dRedemptionPrice</code>.
 * Typically, this amount is less than the notional.
 * @param dRepurchasePrice The repurchase price of the bond given
 * as a percentage of the notional. After the coupon payment,
 * the issuer of the bond can buy the bond back from the holder
 * for the amount <code>rCouponBond.notional *
 * dRepurchasePrice</code>. Usually, this amount is greater than
 * the notional.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction putCallBond (const cfl::Data::CashFlow &rCouponBond,
                                double dRedemptionPrice,
                                double dRepurchasePrice,
                                cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>American put option on futures
 * price of zero-coupon bond</strong>. In this contract, the holder of
 * the option can exercise it at any time from the set:
 * \f[
 * t_i = t_0 + i\frac{T-t_0}{M}, \quad i=1,\dots,M,
 * \f]
 * where \f$t_0=\f$ \p rModel.initialTime(), \f$T=\f$ \p
 * dFuturesMaturity, and \f$M=\f$ \p iFuturesTimes.  He then
 * receives strike \p dStrike and pays the futures price.
 *
 * The futures contract is written on the zero-coupon bond with
 * notional \p dNotional and maturity \p dBondMaturity. The delivery
 * time of the contract is given by \p dFuturesMaturity.
 *
 * @param dNotional The notional amount of the underlying
 * zero-coupon bond.
 * @param dBondMaturity The maturity of the underlying zero-coupon
 * bond.
 * @param dFuturesMaturity The maturity of the futures contract.
 * @param iFuturesTimes The number of "futures" times, that is the
 * times when futures transactions in the contract take place. The
 * set of futures times coincides with the set of exercise
 * times. This set includes the
 * delivery time \p dFuturesMaturity, but does not contain the
 * initial time of the model (the issue time of the contract).
 * @param dStrike The strike of the option.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
americanPutOnFutures (double dNotional, double dBondMaturity,
                      double dFuturesMaturity, unsigned iFuturesTimes,
                      double dStrike, cfl::InterestRateModel &rModel);

/** @} */
}

#endif // of __Homework5_hpp__
