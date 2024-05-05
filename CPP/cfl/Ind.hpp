#ifndef __cflInd_hpp__
#define __cflInd_hpp__

/**
 * @file Ind.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief One-dimensional indicator function.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/Error.hpp"
#include <memory>
#include <valarray>

namespace cfl
{
/**
 * @ingroup cflNumeric
 *
 * @defgroup cflInd Indicators of one-dimensional functions.
 *
 * This module contains efficient implementations of one-dimensional
 * indicator functions.
 *
 * @{
 */

/**
 * @brief  The interface class for one-dimensional indicator function.
 *
 * This is the abstract class for one-dimensional indicator
 * function. Its implementation is used to construct concrete class
 * Ind. The goal is to accelerate numerical integration of
 * discontinuous functions.
 *
 * @see Ind and NInd
 */
class IInd
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IInd () {}

  /**
   * Constructs the indicator function of the event: "the function
   * is greater then the barrier". The goal is to accelerate the
   * numerical integration of discontinuous functions.
   *
   * @param rValues Before the operation, \p rValues represents the
   * values of input function \f$f\f$ on the grid. After the
   * operation, \p rValues represents the indicator of the event
   * \f$\{f(x) \geq b\}\f$, where \f$b = \f$ \p  dBarrier.
   * @param dBarrier The barrier.
   */
  virtual void indicator (std::valarray<double> &rValues,
                          double dBarrier) const = 0;
};

/**
 * @brief  The concrete class for indicator functions.
 *
 * This is the standard class for indicator functions. It is
 * implemented by a new instance of the interface class \p IInd. The
 * class \p Ind is used to define discontinuous functions on a grid
 * with the goal to accelerate the numerical integration.
 *
 * @see IInd and NInd
 */
class Ind
{
public:
  /**
   * A constructor.
   *
   * @param pNewInd Pointer to new implementation of \p IInd.
   */
  explicit Ind (IInd *pNewInd = 0);

  /**
   * @copydoc IInd::indicator()
   */
  void indicator (std::valarray<double> &rValues, double dBarrier) const;

private:
  std::shared_ptr<IInd> m_pInd;
};

/**
 * @brief Implementations of one-dimensional indicator functions.
 *
 * @see IInd and Ind
 */
namespace NInd
{
/**
 * The naive indicator functions with values 0 and 1.
 *
 * @return cfl::Ind
 */
cfl::Ind naive ();

/**
 * The indicator function that depends linearly on the point of
 * discontinuity.
 *
 * @return cfl::Ind
 */
cfl::Ind linear ();

/**
 * The indicator function that has a quadratic dependence on the point
 * of discontinuity.
 *
 * @return cfl::Ind
 */
cfl::Ind quadratic ();
} // namespace NInd
/** @} */
} // namespace cfl

#include "cfl/Inline/iInd.hpp"
#endif // of __cflInd_hpp__
