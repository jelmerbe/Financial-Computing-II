#ifndef __test_all_Numeric_hpp__
#define __test_all_Numeric_hpp__

/**
 * @file Numeric.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Collections of testing functions.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "test/Print.hpp"

namespace test
{
/**
 * @ingroup test_all
 *
 * @{
 */

/**
 * Returns distribution function for the gaussian distribution with
 * mean \p dMean and standard deviation \p dVar.
 *
 * @param dMean The mean.
 * @param dSigma The standard deviation.
 * @return std::function<double(double)>
 */
std::function<double (double)> gauss_cdf (double dMean, double dSigma);

/**
 * Returns the number of nodes for approximation as a
 * function of the interval \f$dX\f$ using the formula:
 *
 * <ol>
 * <li> returns 1 if \f$dX=0\f$;
 * <li> returns the ceiling of
 * \f$ 2.5 + (1 + \sqrt{dQ}) dX / (1 + dX) \f$ if \f$dX>0\f$.
 * </ol>
 *
 * @param dQuality (\f$dQ\f$) The quality parameter.
 * @return std::function<unsigned(double)> The number of nodes for
 * approximation as a function of the interval.
 */
std::function<unsigned (double)> sizeApprox (double dQuality);
/** @} */
} // namespace test

#endif // of __test_all_Numeric_hpp__
