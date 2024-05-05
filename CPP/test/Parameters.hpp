#ifndef __test_all_Parameters_h__
#define __test_all_Parameters_h__

/**
 * @file Parameters.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Default parameters.
 * @version 1.0
 * @date 2023-07-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cfl/Data.hpp"
#include "test/Print.hpp"

namespace test
{
/**
 * The default interest rate.
 *
 */
const double c_dYield = 0.07;

/**
 * The default spot price.
 *
 */
const double c_dSpot = 100;

/**
 * The default dividend yield.
 *
 */
const double c_dDividendYield = 0.02;

/**
 * The default initial time.
 *
 */
const double c_dInitialTime = 0.;

/**
 * The default maturity.
 *
 */
const double c_dMaturity = c_dInitialTime + 1.;

/**
 * The default notional.
 *
 */
const double c_dNotional = 100.;

/**
 * The default interval for relative changes in the output.
 *
 */
const double c_dInterval = 0.2;

/**
 * The default number of values in the output.
 *
 */
const unsigned c_iPoints = 10;

/**
 * The default interval between two payments.
 *
 */
const double c_dPeriod = 0.25;

/**
 * The default number of payments.
 *
 */
const unsigned c_iNumberOfPeriods = 6;

/**
 * The default vector of exercise times.
 *
 * @return std::vector<double>
 */
std::vector<double> exerciseTimes ();

/**
 * The default vector of barrier times.
 *
 * @return std::vector<double>
 */
std::vector<double> barrierTimes ();

/**
 * Returns some default parameters for an interest rate swap.
 *
 * @return cfl::Data::Swap
 */
cfl::Data::Swap swapParameters ();
} // namespace test

#endif // of __test_all_Parameters_h__
