#ifndef __Review2_hpp__
#define __Review2_hpp__

/**
 * @file Review2.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Review 2: options on a stock.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"

/**
 * @mainpage Review 2: options on a stock.
 */

namespace prb
{
/**
 * @defgroup Review2AssetOptions Options on a stock.
 *
 * This module deals with valuation of options on a single stock.
 * @{
 */

/**
 * Computes the value of the <strong>"BOOST" (Banking On Overall
 * Stability) option</strong>. This contract terminates at the first
 * barrier time when the stock price hits either of the barriers.
 * At the exit time the holder of the option receives the payoff
 * given by the product of notional amount \p dNotional on the
 * percentage of the barrier times the stock price spends between
 * the barriers.  If the price of the stock never leaves the
 * barriers, then at the last barrier time the holder receives
 * notional amount \p dNotional.
 *
 * @param dNotional The notional amount.
 * @param dLowerBarrier The lower barrier.
 * @param dUpperBarrier The upper barrier.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction boost (double dNotional, double dLowerBarrier,
                          double dUpperBarrier,
                          const std::vector<double> &rBarrierTimes,
                          cfl::AssetModel &rModel);

/**
 * Computes the price of the <strong>clique option</strong>. The payoff
 * of the option at maturity \p dMaturity is given by:
 * \f[
 * V(T) = \frac1{N} \sum_{i=1}^N \max(S(t_i) - K, 0),
 * \f]
 * where the summation is done over the set of averaging times
 * \p rAverageTimes,  \f$S(t)\f$ is the stock price at \f$t\f$,
 * and \f$K\f$ is the strike \p dStrike.
 *
 * @param dMaturity The maturity of the option.
 * @param rAverageTimes The vector of averaging times. The last averaging
 * time is less than the maturity.
 * @param dStrike The strike.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction clique (double dMaturity,
                           const std::vector<double> &rAverageTimes,
                           double dStrike, cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>simple swing option with
 * penalty</strong>.  This is a simplified version of contracts
 * traded on natural gas.  A holder is given the right to request
 * the delivery of one stock (some pre-specified amount of natural
 * gas) at any of the exercise times. The payment for the delivery
 * is taking place at maturity \p dMaturity and is given by strike
 * \p dStrike.  If the holder will not request the delivery of a
 * stock, then, at maturity, he has to pay penalty \p dPenalty.
 *
 * @param dStrike The strike of the option paid at maturity.
 * @param dPenalty ``No exercise'' penalty paid at maturity.
 * @param rExerciseTimes The vector of exercise times.
 * @param dMaturity The maturity of the option. It is greater than
 * the last exercise time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
simpleSwingWithPenalty (double dStrike, double dPenalty,
                        const std::vector<double> &rExerciseTimes,
                        double dMaturity, cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>American chooser option</strong>.  At
 * an exercise time \p rExerciseTimes[i] a holder can select put or call. In
 * this case, he gets the corresponding European put or call with maturity
 * \p dMaturity and the respective strikes \p rPutStrikes[i] or \p
 * rCallStrikes[i]. Only one such choice can be made.
 *
 * @param rPutStrikes The strikes of the put option.
 * @param rCallStrikes The strikes of the call option.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param dMaturity The maturity (greater than the last exercise time).
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction americanChooser (const std::vector<double> &rPutStrikes,
                                    const std::vector<double> &rCallStrikes,
                                    const std::vector<double> &rExerciseTimes,
                                    double dMaturity, cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>up-range-out put option</strong>.
 * The option is canceled after first \p iOutTimes barrier times,
 * when the stock price is above \p dUpperBarrier. Otherwise,
 * it behaves as standard European put option with maturity \p dMaturity
 * and strike \p dStrike.
 *
 * @param dUpperBarrier The upper barrier.
 * @param iOutTimes The number of barrier events (stock price is
 * above \p dUpperBarrier) that cancels the option.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time. The last barrier time is less
 * than \p dMaturity.
 * @param dStrike The strike of the put option.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction upRangeOutPut (double dUpperBarrier, unsigned iOutTimes,
                                  const std::vector<double> &rBarrierTimes,
                                  double dStrike, double dMaturity,
                                  cfl::AssetModel &rModel);

/**
 * Computes strike \f$K\f$ of <strong>asset variance swap</strong>.
 * The square of the strike is the forward price paid by the buyer
 * at maturity \p dMaturity in return for the annualized variance of
 * the stock price. In other words, the payoff of the long position
 * in the forward contract at the maturity is given by
 * \f[
 * V(T) = \frac1{T - t_0} \sum_{i=1}^N
 * \left(\ln\left(\frac{S(t_i)}{S(t_{i-1})} \right)\right)^2 - K^2.
 * \f]
 * Here \f$S(t_i)\f$ is the spot price at  time
 * \f[
 *   t_i = t_0 + i \frac{T - t_0}{N}, \quad 1\leq i\leq N,
 * \f]
 * \f$T\f$ is the maturity (\p dMaturity), \f$t_0\f$ is the initial
 * time (\p rModel.initialTime()) and \f$N\f$ is the number of times
 * used in the computation of the variance (\p iNumberOfTimes)
 *
 * @param dMaturity The maturity of the variance swap.
 * @param iNumberOfTimes The number of times participating in the
 * computation of the variance
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The strike of the asset variance swap as the function of the
 * initial values of the state processes in the model.
 */
cfl::MultiFunction varianceSwapStrike (double dMaturity,
                                       unsigned iNumberOfTimes,
                                       cfl::AssetModel &rModel);

/**
 * Computes the value of <strong>asset swaption</strong>.
 * At maturity \p dMaturity
 * the owner of the option has the right to enter into
 * the asset swap contract with the parameters \p rSwap and the
 * issue time \p dMaturity.
 * The settlement times of the swap are given by
 * \code
 * t(i) = dMaturity + i * rSwap.period, i=1,...,rSwap.numberOfPayments
 * \endcode
 * At a settlement time <code> t(i+1)</code>
 * <ol>
 * <li> one side pays <em>float</em> interest (float rate + spread) given by
 * \code
 * rSwap.notional*(L(t(i),t(i)+rSwap.period)+rSwap.rate)*rSwap.period
 * \endcode
 * where <code>L(s,t)</code>  is the float rate computed at <code>s</code>
 * for maturity
 * <code>t</code> and <code>rSwap.rate</code> is the spread over float rate.
 * <li>  other side pays the return on the notional amount
 * <code>rSwap.notional</code> produced by the stock between
 * <code>t(i)</code> and <code>t(i+1)</code>, that is,
 * the amount
 * \code
 * rSwap.notional * (S(t(i+1)) - S(t_i))/S(t(i))
 * \endcode
 * </ol>
 *
 * @param rSwap The parameters of the underlying swap contract. Here,
 * <code>rSwap.rate</code> denotes the spread over float rate.
 * @param dMaturity The maturity of the swaption. This is also the issue
 * time for the asset swap.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction assetSwaption (const cfl::Data::Swap &rSwap,
                                  double dMaturity, cfl::AssetModel &rModel);

/**
 * Computes the value of foreign exchange cancellable swap. In this
 * contract, at initial time we pay notional EUR (foreign currency) and
 * receive notional USD (domestic currency).  Later at payment times we
 * pay fixed rate USD and receive float plus spread EUR.  At payment
 * times we have the right to terminate swap. In this case, the last
 * transaction takes place at the next payment time and consists of
 * interest payments and notional amounts. In cfl::AssetModel the
 * forward curve defines the forward exchange rate, where exchange rate
 * is the number of units of domestic currency needed to buy one unit of
 * foreign currency.
 *
 * @param dDomesticNotional The notional amount in domestic currency.
 * @param dFixedRate The fixed rate in the swap (domestic currency).
 * @param dForeignNotional The notional amount in foreign currency.
 * @param dFloatSpread The spread for float (foreign) rate.
 * @param dPeriod The time interval between two payments
 * (as year fraction).
 * @param iNumberOfPayments The number of payments.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction fxCancellableSwap (double dDomesticNotional,
                                      double dFixedRate,
                                      double dForeignNotional,
                                      double dFloatSpread, double dPeriod,
                                      unsigned iNumberOfPayments,
                                      cfl::AssetModel &rModel);
/** @} */
}

#endif // of __Review2_hpp__
