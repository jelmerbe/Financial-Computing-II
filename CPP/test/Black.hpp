#ifndef __test_all_Black_h__
#define __test_all_Black_h__

/**
 * @file Black.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of Black models.
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/BlackModel.hpp"
#include "test/AssetModel.hpp"

namespace test
{
/**
 * @brief Testing in Black model.
 *
 */
namespace Black
{
/**
 *
 * @defgroup test_all_Black Testing in Black model.
 *
 * This module contains testing functions for
 * the Black model on a single stock.
 *
 * @{
 */

/**
 * The default volatility of spot prices.
 *
 */
const double c_dSigma = 0.2;

/**
 * The default mean-reversion rate.
 *
 */
const double c_dLambda = 0.05;

/**
 * Constructs data for Black model on a single stock.
 *
 * @param sModel The printed title.
 * @param dYield The constant interest rate.
 * @param dSpot The spot price.
 * @param dDividendYield The dividend yield.
 * @param dSigma The spot volatility.
 * @param dLambda The mean-reversion rate.
 * @param dInitialTime The initial time.
 * @return cfl::Black::Data
 */
cfl::Black::Data data (const char *sModel = "PARAMETERS OF BLACK MODEL:",
                       double dYield = c_dYield, double dSpot = c_dSpot,
                       double dDividendYield = c_dDividendYield,
                       double dSigma = c_dSigma, double dLambda = c_dLambda,
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
 * @return cfl::AssetModel
 */
cfl::AssetModel model (double dStepQuality = c_dStepQuality,
                       double dWidthQuality = c_dWidthQuality);

/** @} */
} // namespace Black
} // namespace test

#endif // of __test_all_Black_h__
