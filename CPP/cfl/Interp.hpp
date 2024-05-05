#ifndef __cflInterp_hpp__
#define __cflInterp_hpp__

/**
 * @file Interp.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Interpolation of one-dimensional functions.
 * @version 1.0
 * @date 2023-05-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cfl/Function.hpp"
#include <vector>

namespace cfl
{
/**
 * @ingroup cflNumeric
 *
 * @defgroup cflInterp Interpolation of one-dimensional functions.
 *
 * This module deals with one-dimensional interpolation.
 * @{
 */

/**
 * @brief  The interface class for numerical interpolation.
 *
 * This is the interface class for one-dimensional interpolation. Its
 * implementation is used to construct concrete class Interp.
 *
 * @see Interp and NInterp
 */
class IInterp
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IInterp () {}

  /**
   * Returns a pointer to a new implementation of IInterp given the
   * the vectors of arguments and values.
   *
   * @param rArg The strictly increasing vector of arguments of
   * the interpolated function.
   * @param rVal The vector of values of the interpolated function.
   * @return The pointer to a new implementation of IInterp.
   */
  virtual IInterp *newObject (const std::vector<double> &rArg,
                              const std::vector<double> &rVal) const
      = 0;

  /**
   * Returns the interpolated function.
   *
   * @return The interpolated function.
   */
  virtual Function interp () const = 0;

  /**
   * Returns the first derivative of the interpolated function.
   *
   * @return The first derivative of the interpolated function.
   */
  virtual Function deriv () const = 0;

  /**
   * Returns the second derivative of the interpolated function.
   *
   * @return The second derivative of the interpolated function.
   */
  virtual Function deriv2 () const = 0;
};

/**
 * @brief  The concrete class for one-dimensional interpolation.
 *
 * This is the standard class for one-dimensional interpolation.
 * It is constructed from a new implementation of IInterp.
 *
 * @see IInterp and NInterp
 */
class Interp
{
public:
  /**
   * A constructor.
   * @param pNewP A dynamically allocated implementation of IInterp.
   */
  explicit Interp (IInterp *pNewP = 0);

  /**
   * Constructs the one-dimensional interpolated function given the
   * vectors of arguments and values.  The vectors have identical
   * sizes. Arguments are strictly increasing.
   *
   * @param itArgBegin The iterator to the start of the container of arguments.
   * @param itArgEnd The iterator to the end of the container of arguments.
   * @param itValBegin The iterator to the start of the container of values.
   */
  template <class It1, class It2>
  void assign (It1 itArgBegin, It1 itArgEnd, It2 itValBegin);

  /**
   * @copydoc IInterp::interp()
   */
  Function interp () const;

  /**
   * @copydoc IInterp::deriv()
   */
  Function deriv () const;

  /**
   * @copydoc IInterp::deriv2()
   */
  Function deriv2 () const;

private:
  std::shared_ptr<IInterp> m_uP;
};

/**
 * @brief Implementations of one-dimensional interpolations.
 *
 * All interpolation routines become the linear interpolation
 * if the sizes of the input vectors of arguments and values
 * are too small.
 *
 * @see IInterp and Interp
 */
namespace NInterp
{
/**
 * Linear interpolation.
 *
 * @return The engine for linear interpolation.
 */
cfl::Interp linear ();

/**
 * Cubic spline interpolation.
 *
 * @return The engine for cubic spline interpolation.
 */
cfl::Interp cspline ();

/**
 * Steffen interpolation produces monotone function between
 * interpolation nodes.
 *
 * @return The engine for Steffen interpolation.
 */
cfl::Interp steffen ();

/**
 * Akima interpolation is a variant of cubic spline with
 * more tolerance to outliers.
 *
 * @return The engine for Akima interpolation.
 */
cfl::Interp akima ();

/**
 * Polynomial interpolation.
 *
 * @return The engine for polynomial interpolation.
 */
cfl::Interp polynomial ();
} // namespace NInterp
/** @} */
} // namespace cfl

#include "cfl/Inline/iInterp.hpp"
#endif // of __cflInterp_hpp__
