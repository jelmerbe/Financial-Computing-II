#ifndef __test_all_AssetModel_h__
#define __test_all_AssetModel_h__

/**
 * @file AssetModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of options on a single stock.
 * @version 1.0
 * @date 2023-01-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cfl/AssetModel.hpp"
#include "test/Parameters.hpp"

namespace test
{
/**
 * @brief Testing of options on a single stock.
 *
 */

/**
 *
 * @defgroup test_all_AssetModel Testing in a single stock model.
 *
 * This module contains testing functions for a single stock model.
 *
 * @{
 */

/**
 * The default relative error for standard options on a stock.
 */
const double c_dAssetStdRelErr = 10E-4;

/**
 * Prints results of valuation of an option on a single stock.
 *
 * @param f The testing function.
 * @param rModel The reference to AssetModel.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void report (cfl::MultiFunction (*f) (cfl::AssetModel &rModel),
             cfl::AssetModel &rModel, double dRelErr = c_dAssetStdRelErr,
             double dAbsErr = cfl::EPS);

/**
 * Prints the results of valuation of an option on a single stock
 * with a swap-like description.
 *
 * @param f The testing function; \p bPayFloat is a parameter for some
 * swap-like derivatives.
 * @param rModel The reference to AssetModel.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void report (cfl::MultiFunction (*f) (cfl::AssetModel &rModel, bool bPayFloat),
             cfl::AssetModel &rModel, double dRelErr = c_dAssetStdRelErr,
             double dAbsErr = cfl::EPS);

/** @} */
} // namespace test

#endif // of __test_all_AssetModel_h__
