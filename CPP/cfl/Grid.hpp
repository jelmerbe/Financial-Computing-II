#ifndef __cflGrid_hpp__
#define __cflGrid_hpp__

/**
 * @file Grid.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Construction of one-dimensional grid.
 * @version 1.0
 * @date 2023-07-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "GaussRollback.hpp"
#include "Ind.hpp"
#include "Interp.hpp"
#include "Slice.hpp"

namespace cfl
{
/**
 * @ingroup cflCommonElements
 *
 * @defgroup cflGrid One-dimensional grid.
 *
 * This module contains functions used in the construction of one-dimensional
 * grid.
 * @{
 */

/**
 * @brief Construction of a one-dimensional grid.
 *
 */
namespace Grid
{
/**
 * Computes the width of the grid \f$y\f$ as a function of the variance
 * \f$\Sigma^2\f$ taking into account the desired quality \f$Q\f$:
 * \f[
 *  E\left[e^{X}I(X>y/2)\right] \leq \frac1{Q^2},
 * \f]
 * where \f$X\f$ is the gaussian random variable with mean \f$0\f$ and
 * variance \f$\Sigma^2\f$.
 *
 * @param dWidthQuality (\f$Q\f$) The quality parameter.
 * @return The width of the grid as a function of the variance.
 */
std::function<double (double)> widthGauss (double dWidthQuality);

/**
 * Computes the step \f$h\f$ on the grid as the function of the
 * variance \f$\Sigma^2\f$ by the formula:
 * \f[
 * h(\Sigma^2) =
 * \min\left(\frac1{Q},\Sigma\sqrt{\frac{3}{2N}}\right).
 * \f]
 *
 * @param dStepQuality (\f$Q\f$) The quality parameter.
 * @param iUniformSteps (\f$N\f$) The minimal number of steps
 * in the explicit scheme with uniform weights.
 * @return The step on the grid as a function of the variance.
 */
std::function<double (double)> step (double dStepQuality,
                                     unsigned iUniformSteps);
/**
 * Returns the smallest integer greater than given real number.
 *
 * @return The round-off of the approximate size of the grid.
 */
std::function<unsigned (double)> size ();

/**
 * Returns the smallest integer of the form \f$2^n\f$ grater than
 * given real number.
 *
 * @return The round-off of the approximate size of the grid
 * as \f$2^n\f$.
 */
std::function<unsigned (double)> size2 ();
}
/** @} */
} // namespace cfl

#endif // of __cflGrid_hpp__
