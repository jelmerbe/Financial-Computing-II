#ifndef __cfl_RootD_hpp__
#define __cfl_RootD_hpp__

/**
 * @file RootD.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Root finding with polishing algorithm.
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
 * @defgroup cflRootD Root finding with polishing algorithm.
 *
 * This module deals with polishing algorithms for one-dimensional
 * root finding. These algorithms require the computation of both
 * the function and its derivatives.
 *
 * @{
 */
/**
 * @brief The interface class for one-dimensional root finding with
 * polishing algorithm.
 *
 * This is the abstract class for one-dimensional root finding with
 * polishing algorithm. Its implementations are used to construct
 * the concrete class RootD.
 *
 * @see RootD and NRootD
 */
class IRootD
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IRootD () {}

  /**
   * Returns the result of the one-dimensional root finding. We have
   * to supply the initial point \a dX0 for the search.
   *
   * @param rF The input function.
   * @param rDF The derivative of the input function.
   * @param dX0 The initial point of the search.
   * @return The argument \a dX such that \a rF(dX) is zero with the
   * accuracy defined by the search algorithm.
   */
  virtual double find (const cfl::Function &rF, const cfl::Function &rDF,
                       double dX0) const = 0;
};

/**
 * @brief The concrete class for one-dimensional root finding.
 *
 * This is the standard class for one-dimensional root finding. It
 * is implemented by a new instance of IRootD.
 *
 * @see  IRootD and NRootD
 */
class RootD
{
public:
  /**
   * Constructs a one-dimensional root finding scheme from
   * an implementation of IRootD.
   *
   * @param pNewP A pointer to a new instance of IRootD.
   */
  explicit RootD (IRootD *pNewP = 0);

  /**
   * @copydoc IRootD::find()
   */
  double find (const cfl::Function &rF, const cfl::Function &rDF,
               double dX0) const;

private:
  std::shared_ptr<IRootD> m_uP;
};

/**
 * @brief Implementations of one-dimensional root finding routines of polishing
 * type.
 *
 * @see  IRootD and RootD
 */
namespace NRootD
{
/**
 * The classical Newton algorithm of polishing the roots of
 * the function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |x_1-x_0| < \epsilon + \delta |x_1|,
 * \f]
 * where \f$x_0\f$ and \f$x_1\f$ are the previous and the current root
 * estimates. We stop after \a iMaxSteps even if the desired accuracy has not
 * been reached.
 *
 * @param dAbsErr  (\f$\epsilon\f$) The absolute error.
 * @param dRelErr (\f$\delta\f$) The relative error.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The Newton algorithm.
 */
cfl::RootD newton (double dAbsErr, double dRelErr, unsigned iMaxSteps = IMAX);

/**
 * The classical Newton algorithm of polishing the roots of
 * the function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(x_1)| < \epsilon,
 * \f]
 * where \f$x_1\f$ is the current root estimate. We stop after \a iMaxSteps
 * even if the desired accuracy has not been reached.
 *
 * @param dFuncErr  (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The Newton algorithm.
 */
cfl::RootD newton (double dFuncErr, unsigned iMaxSteps = IMAX);

/**
 * The secant algorithm of polishing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |x_1-x_0| < \epsilon + \delta |x_1|,
 * \f]
 * where \f$x_0\f$ and \f$x_1\f$ are the previous and the current root
 * estimates. We stop after \a iMaxSteps even if the desired accuracy has not
 * been reached.
 *
 * @param dAbsErr  (\f$\epsilon\f$) The absolute error.
 * @param dRelErr (\f$\delta\f$) The relative error.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The secant algorithm.
 */
cfl::RootD secant (double dAbsErr, double dRelErr, unsigned iMaxSteps = IMAX);

/**
 * The secant algorithm of polishing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(x_1)| < \epsilon,
 * \f]
 * where \f$x_1\f$ is the current root estimate. We stop after \a iMaxSteps
 * even if the desired accuracy has not been reached.
 *
 * @param dFuncErr  (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The secant algorithm.
 */
cfl::RootD secant (double dFuncErr, unsigned iMaxSteps = IMAX);

/**
 * The Steffenson algorithm of polishing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |x_1-x_0| < \epsilon + \delta |x_1|,
 * \f]
 * where \f$x_0\f$ and \f$x_1\f$ are the previous and the current root
 * estimates. We stop after \a iMaxSteps even if the desired accuracy has not
 * been reached.
 *
 * @param dAbsErr  (\f$\epsilon\f$) The absolute error.
 * @param dRelErr (\f$\delta\f$) The relative error.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The Steffenson algorithm.
 */
cfl::RootD steffenson (double dAbsErr, double dRelErr,
                       unsigned iMaxSteps = IMAX);

/**
 * The Steffenson algorithm of polishing the roots of the
 * function. We achieve the desired accuracy and stop as soon as
 * \f[
 *  |f(x_1)| < \epsilon,
 * \f]
 * where \f$x_1\f$ is the current root estimate. We stop after \a iMaxSteps
 * even if the desired accuracy has not been reached.
 *
 * @param dFuncErr  (\f$\epsilon\f$) The absolute residual error of the
 * function.
 * @param iMaxSteps The maximal number of iterations.
 * @return cfl::RootD The Steffenson algorithm.
 */
cfl::RootD steffenson (double dFuncErr, unsigned iMaxSteps = IMAX);
} // namespace NRootD
/** @} */
} // namespace cfl

#include "cfl/Inline/iRootD.hpp"
#endif // of __cfl_RootD_hpp__
