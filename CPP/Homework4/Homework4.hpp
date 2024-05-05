#ifndef __Homework4_hpp__
#define __Homework4_hpp__

/**
 * @file Homework4.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Homework 4: standard and barrier options on a stock.
 * @version 1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/AssetModel.hpp"
#include "cfl/Data.hpp"

/**
 * @mainpage Homework 4: standard and barrier options on a stock.
 */

namespace prb
{
/**
 * @defgroup Homework4AssetOptions Options on a stock.
 *
 * This module deals with valuation of options on a stock.
 * @{
 */

/**
 * Computes the value of the <strong>European straddle
 * option</strong>.  In this contract, at maturity \p dMaturity a
 * holder of the option can either buy or sell the stock at strike
 * \p dStrike. In other words, the straddle option equals the sum of
 * the European put and call options with the same strike and
 * maturity.
 *
 * @param dStrike The strike of the option.
 * @param dMaturity The maturity of the option.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction straddle (double dStrike, double dMaturity,
                             cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>American call on
 * forward</strong>.  In this contract, at any exercise time (from
 * \p rExerciseTimes) a holder of the option has the right to enter
 * a long position in the forward contract with forward price \p
 * dForwardPrice and time to maturity \p dTimeToMaturity.
 *
 * @param dForwardPrice The forward price.
 * @param dTimeToMaturity The time to maturity of forward contract.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction
americanCallOnForward (double dForwardPrice, double dTimeToMaturity,
                       const std::vector<double> &rExerciseTimes,
                       cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>down-and-rebate
 * option</strong>. The option pays notional \p dNotional at the
 * first barrier time when the spot price is below lower barrier \p
 * dLowerBarrier. Otherwise, the option expires worthless.
 *
 * @param dLowerBarrier The lower barrier.
 * @param dNotional The notional amount.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction downRebate (double dLowerBarrier, double dNotional,
                               const std::vector<double> &rBarrierTimes,
                               cfl::AssetModel &rModel);

/**
 * Computes the value of the <strong>up-and-in American
 * put</strong>.  The option becomes American put with strike \p
 * dStrike and exercise times \p rExerciseTimes after the first
 * barrier time when the stock price is above barrier \p
 * dBarrier. If the price of the stock stays below \p dBarrier for
 * all barrier times, then the option expires worthless.
 *
 * @param dBarrier The upper barrier.
 * @param rBarrierTimes The vector of barrier times. The first
 * time is greater than the initial time.
 * @param dStrike The strike of the option.
 * @param rExerciseTimes The vector of exercise times. The first exercise
 * time is  greater than the initial time.
 * @param rModel The reference to an implementation of cfl::AssetModel.
 *
 * @return The price of the option as the function of the initial
 * values of the state processes in the model.
 */
cfl::MultiFunction upInAmericanPut (double dBarrier,
                                    const std::vector<double> &rBarrierTimes,
                                    double dStrike,
                                    const std::vector<double> &rExerciseTimes,
                                    cfl::AssetModel &rModel);

/** @} */
}

#endif // of __Homework4_hpp__
