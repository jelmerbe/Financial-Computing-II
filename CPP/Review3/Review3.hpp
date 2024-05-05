#ifndef __Review3_hpp__
#define __Review3_hpp__

/**
 * @file Review3.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Review 3: options on interest rates.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Data.hpp"
#include "cfl/InterestRateModel.hpp"

/**
 * @mainpage Review 3: options on interest rates.
 */

namespace prb
{
/**
 * @defgroup Review3InterestRateOptions Options on interest rates.
 *
 * This module deals with valuation of standard and barrier
 * options on interest rates.
 * @{
 */

/**
 * Computes the price of <strong>forward swap lock</strong>.
 * In this contract, at maturity \p dMaturity the holder of the
 * contract enters the interest rate swap with the
 * parameters defined in \p rSwap and the issue time \p dMaturity.
 *
 * @param rSwap The parameters of the underlying swap.
 * @param dMaturity The maturity of the forward contract.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction forwardSwapLock (const cfl::Data::Swap &rSwap,
                                    double dMaturity,
                                    cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>American put on forward rate
 * agreement</strong>. In this contract, at any exercise time <em>t(i)</em>
 * holder of the option has the right to borrow notional amount \p
 * dNotional at interest rate \p dFixedRate for the period \p dPeriod.
 *
 * @param dFixedRate The fixed interest rate.
 * @param dPeriod The period of interest rate loan.
 * @param dNotional The notional amount.
 * @param rExerciseTimes The vector of exercise times. The first
 * time is strictly greater than the initial time of the model.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction americanPutOnFRA (double dFixedRate, double dPeriod,
                                     double dNotional,
                                     const std::vector<double> &rExerciseTimes,
                                     cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>up-and-out interest rate
 * floor</strong>.
 * The option makes the same payments as the standard interest rate floor
 * with parameters \p rFloor until the first payment time
 * <code>t(i)</code>,  when the float rate is above the upper barrier
 * \p dUpperBarrier. Then  the payment at <code>t(i)</code> takes
 * place and the contract is terminated.
 *
 * @param rFloor The parameters of the underlying interest rate
 * floor.
 * @param dUpperBarrier The upper barrier for float rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction upOutFloor (const cfl::Data::CashFlow &rFloor,
                               double dUpperBarrier,
                               cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>callable capped floater</strong>.
 * In this contract, at an exercise time \f$t_i\f$ the holder of the
 * option receives the coupon
 * \f[
 *    N \delta t \times \min(L(t_{i-1},t_i)+ \delta L, C)
 * \f]
 * where \f$L(s,t)\f$ is the float rate computed at \f$s\f$ for maturity
 * \f$t\f$, \f$N\f$  is the notional amount (\p rCap.notional),
 * \f$\delta L\f$ is the spread over float rate (\p dRateSpread),
 * \f$C\f$ is the cap rate (\p rCap.rate) and
 * \f$\delta t = t_{i} - t_{i-1}\f$
 * is the time interval between two payments (\p rCap.period).
 * The seller can terminate the contract at any exercise time.
 * In this case, in addition to the above coupon he pays the
 * notional. If the contract has not been terminated before,
 * then at maturity the holder receives the above coupon plus notional.
 *
 * @param rCap The parameters of the contract. Here <code>rCap.rate</code>
 *  defines the cap rate.
 * @param dRateSpread The spread over float rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction callableCappedFloater (const cfl::Data::CashFlow &rCap,
                                          double dRateSpread,
                                          cfl::InterestRateModel &rModel);

/**
 * Computes the <strong>futures price of  the cheapest bond to
 * deliver</strong>.
 * The set of times, when the futures price is evaluated is given by
 * \f[
 * t_i = t_0 + i\frac{T-t_0}{M}, \quad i=1,\dots,M,
 * \f]
 * where \f$t_0=\f$ \p rModel.initialTime(), \f$T=\f$ \p
 * dFuturesMaturity, and \f$M=\f$ \p iFuturesTimes.  At the
 * maturity, the seller of the contract can deliver any bond from \p
 * rBonds.
 *
 * @param dFuturesMaturity The maturity of the futures contract.
 * @param iFuturesTimes The number of futures times, that is, the
 * times when the futures price is determined.
 * @param rBonds The parameters of the underlying bonds. We assume
 * that the issue times for all bonds equal \p dFuturesMaturity.
 * @param rModel The reference to the implementation of
 * cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
futuresOnCheapToDeliver (double dFuturesMaturity, unsigned iFuturesTimes,
                         const std::vector<cfl::Data::CashFlow> &rBonds,
                         cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>puttable bond with resettable
 * coupon</strong> at its issue time. In this contract, after a
 * coupon payment the issuer can reset the coupon rate from original
 * (higher) value <code>rBond.rate</code> to (lower) reset value \p
 * dResetCouponRate. After that, at any payment time greater or
 * equal the reset time and less than the maturity, the holder can
 * sell the bond back to the issuer for redemption value
 * <code>dRedemptionPrice*rBond.notional</code>.
 *
 * @param rBond The parameters of the underlying coupon bond. The
 * initial coupon rate is given by <code>rBond.rate</code>.
 * @param dResetCouponRate The coupon rate after reset. Usually,
 * this rate is smaller than the original coupon rate.
 * @param dRedemptionPrice The redemption price of the bond as the percentage
 * of the notional.  Typically, <code>dRedemptionPrice<1</code>.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction resetCouponPutBond (const cfl::Data::CashFlow &rBond,
                                       double dResetCouponRate,
                                       double dRedemptionPrice,
                                       cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>constant maturity
 * swaption</strong>.  At maturity \p dMaturity a holder of the
 * option can enter into the underlying CMS (constant maturity
 * swap) agreement. The maturity \p dMaturity becomes the issue
 * time of CMS. In the CMS the float interest is paid according to
 * the current swap rate in the swap contract which expires after
 * \p iCMSPeriods.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param dMaturity The maturity of the option.
 * @param iCMSPeriods The number of periods in the swap contract
 * which determines the float rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction CMSwaption (const cfl::Data::Swap &rSwap, double dMaturity,
                               unsigned iCMSPeriods,
                               cfl::InterestRateModel &rModel);

/**
 * Computes the price of <strong>American constant maturity
 * swaption</strong>.  A holder of the option can enter into the
 * underlying CMS (constant maturity swap) agreement at any
 * exercise time. This time then becomes the issue time of CMS. In
 * the CMS the float interest is paid according to the market
 * swap rate in the swap contract expiring after \p iCMSPeriods.
 *
 * @param rSwap The parameters of the underlying interest rate
 * swap.
 * @param rExerciseTimes The set of exercise times. The first
 * exercise time is strictly greater than the initial time in the
 * model.
 * @param iCMSPeriods The number of periods in the swap contract
 * that determines the float rate.
 * @param rModel The reference to the implementation of cfl::InterestRateModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
americanCMSwaption (const cfl::Data::Swap &rSwap,
                    const std::vector<double> &rExerciseTimes,
                    unsigned iCMSPeriods, cfl::InterestRateModel &rModel);
/** @} */
}

#endif // of __Review3_hpp__
