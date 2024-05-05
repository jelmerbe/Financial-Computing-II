#ifndef __cflBrownian_hpp__
#define __cflBrownian_hpp__

/**
 * @file Brownian.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Financial model driven by Brownian motion.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "GaussRollback.hpp"
#include "Grid.hpp"
#include "Ind.hpp"
#include "Interp.hpp"
#include "Slice.hpp"

namespace cfl
{
/**
 * @ingroup cflCommonElements
 *
 * @defgroup cflBrownian Basic model with Brownian motion.
 *
 * This module contains implementation of the basic financial model
 * where the state process is a one-dimensional Brownian motion.
 *
 * @{
 */

/**
 * Returns cfl::Model representing a Brownain motion given
 * - \a rVar The vector of variances of the Brownian motion.
 *   \a rVar[i] defines the average variance between \a rEventTimes[0]
 *   and \a rEventTimes[i]. This vector has the same size as
 *   the vector of event times.
 * - \a rEventTimes The vector of event times in the model.
 * - \a dInterval The width of the interval of initial values for
     the Brownian motion.
 */
typedef std::function<Model (const std::vector<double> &rVar,
                             const std::vector<double> &rEventTimes,
                             double dInterval)>
    TBrownian;

/**
 * Implements the generator of Brownian model.  The storage in the
 * Brownian model will be in the form of symmetric equally spaced
 * grid with \f$ 2^n \f$ elements (to facilitate the use of radix-2
 * Fast Fourier Transform).
 *
 * @param dStepQuality \f$ q \f$ This parameter defines the step
 * \f$ h \f$ on the grid: \f$ h = 1/q \f$.
 * @param dWidthQuality \f$ r \f$ This parameter defines the width
 * \f$ w \f$ of the grid as a function of the given total variance
 * \f$ \sigma^2 \f$. The width \f$ w \f$ is chosen so that
 * \f[
 * E(e^{\sigma X}I(\sigma X>w/2)) \leq 1/{r^2},
 * \f]
 * where \f$ X \f$ is a standard Gaussian random variable.
 * @param iUniformSteps The minimal number of uniform steps in the explicit
 * scheme between two event times.
 * @param rSize The size of the grid as a function of the total variance.
 * @param rRollback An implementation of the operator of conditional
 * expectation with respect to gaussian distribution.
 * @param rInd A numerically efficient implementation of
 * discontinuous functions.
 * @param rInterp An implementation of numerical interpolation.
 * @return TBrownianModel The constructor of Brownian motion.
 */
TBrownian
brownian (double dStepQuality, double dWidthQuality,
          unsigned iUniformSteps = 3,
          const std::function<unsigned (double)> &rSize = Grid::size2 (),
          const GaussRollback &rRollback = cfl::NGaussRollback::chain (),
          const Ind &rInd = cfl::NInd::linear (),
          const Interp &rInterp = cfl::NInterp::cspline ());

/**
 * Implements the generator of Brownian model.  The storage in the
 * Brownian model will be in the form of symmetric equally spaced
 * grid with \f$ 2^n \f$ elements (to facilitate the use of radix-2
 * Fast Fourier Transform).
 *
 * @param rH Returns the step on the grid as a function of
 * the \em minimal total variance between two event times.
 * @param rWidth The function computes the width of the grid for the
 * given total variance. The width does not include the interval of
 * initial values for the Brownian motion.
 * @param rSize The size of the grid as a function of the total variance.
 * @param rRollback An implementation of the operator of conditional
 * expectation with respect to gaussian distribution.
 * @param rInd A numerically efficient implementation of
 * discontinuous functions.
 * @param rInterp An implementation of numerical interpolation.
 * @return TBrownianModel The constructor of Brownian motion.
 */
TBrownian brownian (const std::function<double (double)> &rH,
                    const std::function<double (double)> &rWidth,
                    const std::function<unsigned (double)> &rSize,
                    const GaussRollback &rRollback, const Ind &rInd,
                    const Interp &rInterp);
/** @} */
} // namespace cfl

#endif // of __cflBrownian_hpp__
