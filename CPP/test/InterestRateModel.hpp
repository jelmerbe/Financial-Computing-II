#ifndef __test_all_InterestRateModel_h__
#define __test_all_InterestRateModel_h__

/**
 * @file InterestRateModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of options on interest rates.
 * @version 1.0
 * @date 2023-01-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cfl/InterestRateModel.hpp"
#include "test/Parameters.hpp"
#include "test/Print.hpp"

namespace test
{
/**
 * @brief Testing of options on interest rates.
 *
 */

/**
 *
 * @defgroup test_all_InterestRateModel Testing in interest rate models.
 *
 * This module contains testing functions for interest rate models.
 *
 * @{
 */

/**
 * The default relative error for standard interest rate options.
 */
const double c_dInterestRateStdRelErr = 10E-5;

/**
 * Prints the results of valuation of an interest rate option.
 *
 * @param f The testing function.
 * @param rModel The reference to InterestRateModel.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void report (cfl::MultiFunction (*f) (cfl::InterestRateModel &rModel),
             cfl::InterestRateModel &rModel,
             double dRelErr = c_dInterestRateStdRelErr,
             double dAbsErr = cfl::EPS);

/**
 * Prints the results of valuation of an interest rate option
 * with a swap-like description.
 *
 * @param f The testing function; \p bPayFloat is a parameter for a
 * swap-like derivative.
 * @param rModel The reference to InterestRateModel.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void report (cfl::MultiFunction (*f) (cfl::InterestRateModel &rModel,
                                      bool bPayFloat),
             cfl::InterestRateModel &rModel,
             double dRelErr = c_dInterestRateStdRelErr,
             double dAbsErr = cfl::EPS);
/** @} */
} // namespace test

#endif // of __test_all_InterestRateModel_h__
