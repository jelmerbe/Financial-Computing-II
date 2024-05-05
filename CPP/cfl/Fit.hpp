#ifndef __cfl_Fit_hpp__
#define __cfl_Fit_hpp__

/**
 * @file Fit.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Fitting of one-dimensional functions.
 * @version 1.0
 * @date 2021-01-26
 *
 * @copyright Copyright (c) 2020
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
 * @defgroup cflFit Fitting of one-dimensional functions.
 *
 * This module deals with least-squares fitting of one-dimensional
 * functions.
 *
 * @{
 */

/**
 * @brief The result of a least-squares fit.
 *
 */
struct FitParam
{
  /**
   * The vector of fitted coefficients.
   *
   */
  std::valarray<double> fit;

  /**
   * The covariance (<code>fit.size() x fit.size()</code>)
   * matrix of fitted coefficients. The storage is
   * row based, that is, the element \p (i,j) has index
   * <code>i*fit.size() + j</code>.
   *
   */
  std::valarray<double> cov;

  /**
   * The total error of the fit.
   *
   */
  double chi2;
};

/**
 * @brief The interface class for one-dimensional fitting.
 *
 * This is the abstract class for one-dimensional fitting. Its
 * implementations are used to construct concrete class Fit.
 *
 * @see Fit and NFit
 */
class IFit
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IFit () {}

  /**
   * Returns a pointer to a new implementation of IFit for the given
   * vectors of arguments, values, and weights.
   *
   * @param rArg The strictly increasing vector of arguments of
   * the fitted function.
   * @param rVal The vector of values of the fitted function.
   * @param rW The vector of fitting weights.
   * @param bChi2 If \p true, then the normalizing standard
   * deviation \f$\kappa\f$ is obtained by the \f$\chi^2\f$ estimate.
   * Otherwise, \f$\kappa=1\f$.
   * @return The pointer to a new implementation of IFit.
   */
  virtual IFit *newObject (const std::vector<double> &rArg,
                           const std::vector<double> &rVal,
                           const std::vector<double> &rW,
                           bool bChi2) const = 0;

  /**
   * Returns the result of the fit.
   *
   * @return The result of the fit.
   */
  virtual Function fit () const = 0;

  /**
   * Returns the error of the fit.
   *
   * @return The error of the fit.
   */
  virtual Function err () const = 0;

  /**
   * Returns the fitted coefficients, their covariance matrix,
   * and the total \f$\chi^2\f$ error.
   *
   * @return FitParam
   */
  virtual FitParam param () const = 0;
};

/**
 * @brief The concrete class for one-dimensional fitting.
 *
 * This is the standard class for one-dimensional fitting. It is
 * implemented by a new instance of IFit.
 *
 * @see  IFit and NFit
 */
class Fit
{
public:
  /**
   * Constructs a least-squares fitting scheme from implementation of IFit.
   *
   * @param pNewP Pointer to a new instance of IFit.
   */
  explicit Fit (IFit *pNewP = 0);

  /**
   * Constructs the fitted one-dimensional function for the  given
   * vectors of arguments and values.  The vectors have identical
   * sizes. The arguments are strictly increasing. The function is
   * equivalent to the function
   * \code
   * cfl::Fit::assign(itArgBegin, itArgEnd, itValBegin, itWt, true)
   * \endcode
   * where all weights are equal to 1. The normalizing standard
   * deviation \f$\kappa\f$ is obtained by the \f$\chi^2\f$ estimate.
   *
   * @param itArgBegin An iterator to the start of the container of arguments.
   * @param itArgEnd An iterator to the end of the container of arguments.
   * @param itValBegin An iterator to the start of the container of values.
   */
  template <class InIt1, class InIt2>
  void assign (InIt1 itArgBegin, InIt1 itArgEnd, InIt2 itValBegin);

  /**
   * Constructs the fitted one-dimensional function for the given
   * vectors of arguments, values, and weights. The vector of
   * arguments is strictly increasing. All containers have the same
   * size.
   *
   * @param itArgBegin An iterator to the start of the container of arguments.
   * @param itArgEnd An iterator to the end of the container of arguments.
   * @param itValBegin An iterator to the start of the container of values.
   * @param itWtBegin An iterator to the start of the container of weights.
   * @param bChi2 If \p true, then the normalizing standard
   * deviation \f$\kappa\f$ is obtained by the \f$\chi^2\f$ estimate.
   * Otherwise, \f$\kappa=1\f$.
   */
  template <class InIt1, class InIt2, class InIt3>
  void assign (InIt1 itArgBegin, InIt1 itArgEnd, InIt2 itValBegin,
               InIt3 itWtBegin, bool bChi2 = true);

  /**
   * @copydoc IFit::fit()
   */
  Function fit () const;

  /**
   * @copydoc IFit::err()
   */
  Function err () const;

  /**
   * @copydoc IFit::param()
   */
  FitParam param () const;

private:
  std::shared_ptr<IFit> m_uP;
};

/**
 * @brief Implementations of one-dimensional (least-squares) fits.
 *
 * @see  IFit and Fit
 */
namespace NFit
{
/**
 * The least-squares fit of one-dimensional function \f$f\f$ with a
 * vector of basis functions \f$(g_j)_{j=0,\dots,M-1}\f$ and a free
 * function \f$h\f$.  The family of fitting functions has the form:
 * \f[
 *   f(x, \mathbf{c}) = \sum_{j=0}^{M-1} c_j g_j(x) + h(x),
 * \f]
 * where \f$\mathbf{c} = (c_0,\dots,c_{M-1})\f$ is the vector of
 * the fitting coefficients.
 *
 * @param rBasisF The vector of basis functions
 * \f$(g_j)_{j=0,\dots,M-1}\f$.
 * @param rFreeF The free function \f$h\f$.
 * @return Linear least-squares fitting engine.
 */
cfl::Fit linear (const std::vector<cfl::Function> &rBasisF,
                 const cfl::Function &rFreeF = cfl::Function (0.));

/**
 * The least-squares fit of one-dimensional function \f$f\f$ given a
 * basis function \f$g\f$ and a free function \f$h\f$.  The family of
 * fitting functions has the form:
 * \f[
 *   f(x,c) = cg(x) + h(x),
 * \f]
 * where \f$c\f$ is the fitting coefficient.
 *
 * @param rBasisF The basis function \f$g=g(x)\f$.
 * @param rFreeF The free function \f$h=h(x)\f$.
 * @return Linear least-squares fitting engine.
 */
cfl::Fit linear (const cfl::Function &rBasisF,
                 const cfl::Function &rFreeF = cfl::Function (0.));

/**
 * The least-squares fit of one-dimensional function \f$
 * f=f(x) \f$ given a constant, a basis function \f$g=g(x)\f$, and
 * a free function \f$h=h(x)\f$.  The family of fitting functions
 * has the form:
 * \f[
 *   f(x,c_0,c_1) = c_0 + c_1 g(x) + h(x),
 * \f]
 * where \f$(c_0,c_1)\f$ are the fitting coefficients.
 *
 * @param rBasisF The basis function \f$g=g(x)\f$.
 * @param rFreeF The free function \f$h=h(x)\f$.
 * @return Linear least-squares fitting engine.
 */
cfl::Fit linear_regression (const cfl::Function &rBasisF,
                            const cfl::Function &rFreeF = cfl::Function (0.));

/**
 * Least-squares fitting with basis splines.
 *
 * @param iOrder The order of the fitting spline.
 * @param rBreakpoints The vector of breakpoints.
 * @return Least-squares fitting engine with basis splines.
 */
cfl::Fit bspline (unsigned iOrder, const std::vector<double> &rBreakpoints);

/**
 * Least-squares fitting with basis splines and uniform breakpoints.
 *
 * @param iOrder The order of the fitting spline.
 * @param dL The left point of the domain.
 * @param dR The right point of the  domain.
 * @param iBreakpoints The number of breakpoints.
 * @return Least-squares fitting engine with basis splines.
 */
cfl::Fit bspline (unsigned iOrder, double dL, double dR,
                  unsigned iBreakpoints);
} // namespace NFit
/** @} */
} // namespace cfl

#include "cfl/Inline/iFit.hpp"
#endif // of __cfl_Fit_hpp__
