#ifndef __cfl_Similar_hpp__
#define __cfl_Similar_hpp__

/**
 * @file Similar.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Implementation of models with identical state processes.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cfl/Model.hpp>

namespace cfl
{
/**
 * @ingroup cflCommonElements
 *
 * @defgroup cflSimilar Similar financial models.
 *
 * This module facilitates implementations of financial models sharing
 * the same state process (\a similar models).
 *
 * @see Model
 * @{
 */

/**
 * Implements the rollback operator: replaces \p rSlice with its
 * price at \p iEventTime.
 *
 * - \p rSlice Before the function call it defines the input
 * payoff. After the function call it defines the price of the input
 * payoff at the event time with index \p iEventTime.
 * - \p iEventTime The index of the target event time for the rollback.
 */
typedef std::function<void (Slice &rSlice, unsigned iEventTime)> TRollback;

/**
 * Constructs the similar model given the implementation of rollback
 * operator in the setup of the base model. Deep copies of the inputs
 * are kept inside of the result.
 *
 * @param rTargetRollback Runs the rollback operator of the target model in
 * the framework of the base model.
 * @param rBase A constant reference to the base model.
 * @return Model Implementation of the target model.
 */
Model similar (const TRollback &rTargetRollback, const Model &rBase);

/** @} */
} // namespace cfl

#endif // __cfl_Similar_hpp__
