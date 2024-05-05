#ifndef __test_all_HullWhite_h__
#define __test_all_HullWhite_h__

/**
 * @file HullWhite.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of Hull and White models.
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/HullWhiteModel.hpp"
#include "test/InterestRateModel.hpp"

namespace test
{
/**
 * @brief Testing in Hull and White model.
 *
 */
namespace HullWhite
{
/**
 *
 * @defgroup test_all_HW Testing in Hull and White model.
 *
 * This module contains testing functions for the
 * Hull and White model of interest rates.
 *
 * @{
 */

/**
 * The default volatility of short-term interest rate.
 *
 */
const double c_dSigma = 0.01;

/**
 * The default mean-reversion rate.
 *
 */
const double c_dLambda = 0.02;

/**
 * Returns input data for Hull and White model of interest rates.
 *
 * @param dYield The interest rate.
 * @param dSigma The volatility of short-term interest rate.
 * @param dLambda The mean-reversion rate.
 * @param dInitialTime The initial time.
 * @return cfl::HullWhite::Data
 */
cfl::HullWhite::Data data (double dYield = c_dYield, double dSigma = c_dSigma,
                           double dLambda = c_dLambda,
                           double dInitialTime = c_dInitialTime);

/**
 * The default step quality of model implementation.
 *
 */
const double c_dStepQuality = 200;

/**
 * The default width quality of model implementation.
 *
 */
const double c_dWidthQuality = 100;

/**
 * Default implementation of Black model for testing purposes.
 *
 * @param dStepQuality The step quality of model implementation.
 * @param dWidthQuality The width quality of model implementaion.
 * @return cfl::InterestRateModel
 */
cfl::InterestRateModel model (double dStepQuality = c_dStepQuality,
                              double dWidthQuality = c_dWidthQuality);
/** @} */
} // namespace HullWhite
} // namespace test

#endif // of __test_all_HullWhite_h__
