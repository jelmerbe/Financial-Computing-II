#ifndef __cfl_Root_hpp__
#define __cfl_Root_hpp__

/**
 * @file Root.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Root finding with bracketing algorithm.
 * @version 1.0
 * @date 2022-02-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "cfl/Function.hpp"
#include <valarray>
#include <vector>

namespace cfl
{
/**
 * @ingroup cflNumeric
 *
 * @defgroup cflRoot Root finding with bracketing algorithm.
 *
 * This module deals with bracketing algorithms for one-dimensional
 * root finding. These algorithms only require the computation of
 * function values.
 *
 * @{
 */
/**
 * @brief The interface class for one-dimensional root finding with
 * bracketing algorithm.
 *
 * This is the abstract class for one-dimensional root finding with
 * bracketing algorithm. Its implementations are used to construct
 * the concrete class Root.
 *
 * @see Root and NRoot
 */
class IRoot
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IRoot () {}

  /**
   * Returns the result of the one-dimensional root finding. We
   * supply the initial interval \a [dL,dR] for the search.
   *
   * @param rF The input function.
   * @param dL The left point of the search.
   * @param dR The right point of the search.
   * @return The argument \a dX such that \a rF(dX) is zero with the
   * accuracy defined by the search algorithm.
   */
  virtual double find (const cfl::Function &rF, double dL,
                       double dR) const = 0;
};

/**
 * @brief The concrete class for one-dimensional root finding.
 *
 * This is the standard class for one-dimensional root finding. It
 * is implemented by a new instance of IRoot.
 *
 * @see  IRoot and NRoot
 */
class Root
{
public:
  /**
   * Constructs a one-dimensional root finding scheme from
   * an implementation of IRoot.
   *
   * @param pNewP A pointer to a new instance of IRoot.
   */
  explicit Root (IRoot *pNewP = 0);

  /**
   * @copydoc IRoot::find()
   */
  double find (const cfl::Function &rF, double dL, double dR) const;

private:
  std::shared_ptr<IRoot> m_uP;
};

/**
 * @brief Implementations of one-dimensional root finding routines of
 * bracketing type.
 *
 * @see  IRoot and Root
 */
namespace NRoot
{
/**
 * The classical bisection algorithm of bracketing the roots of
 * the function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |a-b| < \epsilon + \delta \min(|a|,|b||) I(0 \not\in [a,b]),
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dAbsErr (\f$\epsilon\f$) The absolute error for the root.
 * @param dRelErr (\f$\delta\f$) The relative error for the root.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The bisection algorithm.
 */
cfl::Root bisection (double dAbsErr, double dRelErr,
                     unsigned iMaxSteps = IMAX);

/**
 * The classical bisection algorithm of bracketing the roots of
 * the function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(\frac{a+b}2)| < \epsilon,
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dFuncErr (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The bisection algorithm.
 */
cfl::Root bisection (double dFuncErr, unsigned iMaxSteps = IMAX);

/**
 * The false position algorithm of bracketing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |a-b| < \epsilon + \delta \min(|a|,|b||) I(0 \not\in [a,b]),
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dAbsErr (\f$\epsilon\f$) The absolute error for the root.
 * @param dRelErr (\f$\delta\f$) The relative error for the root.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The false position algorithm.
 */
cfl::Root falsepos (double dAbsErr, double dRelErr, unsigned iMaxSteps = IMAX);

/**
 * The false position algorithm of bracketing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(\frac{a+b}2)| < \epsilon,
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dFuncErr (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The bisection algorithm.
 */
cfl::Root falsepos (double dFuncErr, unsigned iMaxSteps = IMAX);

/**
 * The Brent-Dekker algorithm of bracketing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |a-b| < \epsilon + \delta \min(|a|,|b||) I(0 \not\in [a,b]),
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dAbsErr (\f$\epsilon\f$) The absolute error for the root.
 * @param dRelErr (\f$\delta\f$) The relative error for the root.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The Brent-Dekker algorithm.
 */
cfl::Root brent (double dAbsErr, double dRelErr, unsigned iMaxSteps = IMAX);

/**
 * The Brent-Dekker algorithm of bracketing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(\frac{a+b}2)| < \epsilon,
 * \f]
 * where \f$[a,b]\f$ is the current working interval. We stop
 * after \a iMaxSteps even if the desired accuracy has not been reached.
 *
 * @param dFuncErr (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::Root The Brent-Dekker algorithm.
 */
cfl::Root brent (double dFuncErr, unsigned iMaxSteps = IMAX);
} // namespace NRoot
/** @} */
} // namespace cfl

#include "cfl/Inline/iRoot.hpp"
#endif // of __cfl_Root_hpp__
