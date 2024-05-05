#ifndef __Session4_hpp__
#define __Session4_hpp__

/**
 * @file Session4.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Session 4: standard and barrier options on a stock.
 * @version 1
 * @date 2023-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"

/**
 * @mainpage Session 4: standard and barrier options on a stock.
 */

namespace prb
{
/**
 * @defgroup Session4AssetOptions Options on a stock.
 *
 * This module deals with valuation of options on a single stock.
 * @{
 */

/**
 * Computes the value of the <strong>down-and-out call
 * option</strong>. The payoff of
 * the option at maturity equals the payoff of the standard
 * call option with strike \p dStrike and maturity \p dMaturity if
 * the spot price is above barrier \p dBarrier for all barrier
 * times \p rBarrierTimes. Otherwise, the option expires worthless.
 *
 * @param dBarrier The lower barrier.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time. The last barrier time is
 * less than maturity \p dMaturity.
 * @param dStrike The strike of the option.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction downOutCall (double dBarrier,
                                const std::vector<double> &rBarrierTimes,
                                double dStrike, double dMaturity,
                                cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>call on forward price</strong>.
 * In this contract, at maturity \p dCallMaturity the holder of the
 * option can pay strike \p dStrike for the cash amount given by the
 * forward price computed at \p dCallMaturity for delivery at \p
 * dForwardMaturity.
 *
 * @param dStrike The strike of the option.
 * @param dCallMaturity The maturity of the option. It is  greater
 * than the initial time.
 * @param dForwardMaturity The maturity of the forward contract. It is
 * is  greater than \p dCallMaturity.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction callOnForwardPrice (double dStrike, double dCallMaturity,
                                       double dForwardMaturity,
                                       cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>American butterfly
 * option</strong>.  In this contract, at any exercise time (from \p
 * rExerciseTimes) a holder of the option has the right to get the
 * following payoffs:
 *
 * 1. the long call with strike \p dStrike-dStrikeStep,
 * 2. the two short calls with strike \p dStrike,
 * 3. the long call with strike \p dStrike+dStrikeStep.
 *
 * After such an exercise, the option is terminated.
 *
 * @param dStrike The strike in the middle.
 * @param dStrikeStep The length of the wing.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
americanButterfly (double dStrike, double dStrikeStep,
                   const std::vector<double> &rExerciseTimes,
                   cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>"corridor" option</strong>.
 * The last barrier time is the maturity of the option.  The payoff of
 * the option at maturity equals the product of the notional on the
 * percentage of the barrier times, when the price of the stock is less
 * than the upper barrier and is greater than the lower barrier.
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
cfl::MultiFunction corridor (double dNotional, double dLowerBarrier,
                             double dUpperBarrier,
                             const std::vector<double> &rBarrierTimes,
                             cfl::AssetModel &rModel);

/** @} */
} // namespace prb

#endif // of __Session4_hpp__
