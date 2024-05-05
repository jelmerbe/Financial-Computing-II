#ifndef __cflGaussRollback_hpp__
#define __cflGaussRollback_hpp__

/**
 * @file GaussRollback.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Conditional expectation with respect to gaussian distribution.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "Macros.hpp"
#include <memory>
#include <valarray>

namespace cfl
{
/**
 * @ingroup cflNumeric
 *
 * @defgroup cflGaussRollback Gaussian conditional expectation.
 *
 * This module implements conditional expectation with respect to
 * one-dimensional gaussian distribution.
 * @{
 */

/**
 * @brief  The interface class for the operator of conditional
 * expectation with respect to gaussian distribution.
 *
 * An implementation of the interface class IGaussRollback is used
 * to construct the concrete class GaussRollback.
 *
 * @see GaussRollback and NGaussRollback
 */
class IGaussRollback
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IGaussRollback () {}

  /**
   * The virtual constructor.
   * Constructs the numerical scheme that implements the operator of
   * conditional expectation with respect to the gaussian
   * distribution with mean 0 and variance \p dVar on the grid with
   * step \p dH and size \p iSize.
   *
   * @param iSize The number of points on the grid.
   * @param dH The distance between the points on the grid.
   * @param dVar The variance of the gaussian distribution.
   * @return A pointer to new implementation of IGaussRollback.
   */
  virtual IGaussRollback *newObject (unsigned iSize, double dH,
                                     double dVar) const = 0;

  /**
   * Implements the operator of conditional expectation with respect
   * to gaussian distribution.
   *
   * @param rValues \em Before \p rollback this array contains the
   * original values of the function.  \em After \p rollback, it
   * contains their conditional expectations with respect to the
   * gaussian distribution.
   */
  virtual void rollback (std::valarray<double> &rValues) const = 0;
};

/**
 * @brief The concrete class for the operator of conditional expectation
 * with respect to gaussian distribution.
 *
 * This is the concrete class for numerical implementations of the
 * operator of conditional expectation with respect to gaussian
 * distribution. It is constructed by a new implementation of the interface
 * class IGaussRollback.
 *
 * @see IGaussRollback and NGaussRollback
 */
class GaussRollback
{
public:
  /**
   * Constructs a numerical engine that computes conditional
   * expectations with respect to gaussian distribution.
   *
   * @param pNewP Pointer to new implementation of IGaussRollback.
   */
  explicit GaussRollback (IGaussRollback *pNewP = 0);

  /**
   * Resets the parameters of the current GaussRollback object.
   * Constructs a numerical engine that computes the conditional
   * expectation with respect to the gaussian distribution with mean
   * 0 and variance \p dVar on the grid with step \p dH and size \p
   * iSize.
   *
   * @param iSize The number of points on the grid.
   * @param dH The distance between the points on the grid.
   * @param dVar The variance of the gaussian distribution.
   */
  void assign (unsigned iSize, double dH, double dVar);

  /**
   * @copydoc IGaussRollback::rollback()
   */
  void rollback (std::valarray<double> &rValues) const;

  /**
   * Rollback operator that also computes the first derivatives with
   * respect to the state variable.  The first derivatives are
   * computed using the integration by parts formula. In total,
   * rollback runs twice.
   *
   * @param rValues \em Before \p rollback this array contains the
   * original values of the function.  \em After \p rollback, it
   * contains their conditional expectations with respect to the
   * gaussian distribution.
   * @param rDelta Returns the values of the first derivatives of
   * the conditional expectation with respect to the state variable.
   */
  void rollback (std::valarray<double> &rValues,
                 std::valarray<double> &rDelta) const;

  /**
   * Rollback operator that also computes the first and
   * second derivatives with respect to the state variable.
   * The first and second derivatives are computed using the
   * integration by parts formula. In total, the rollback runs 3
   * times.
   *
   * @param rValues \em Before \p rollback this array contains the
   * original values of the function.  \em After \p rollback, it
   * contains their conditional expectations with respect to the
   * gaussian distribution.
   * @param rDelta Returns the values of the first derivatives of
   * the conditional expectation with respect to the state variable.
   * @param rGamma Returns the values of the second derivatives of
   * the conditional expectation with respect to the state variable.
   */
  void rollback (std::valarray<double> &rValues, std::valarray<double> &rDelta,
                 std::valarray<double> &rGamma) const;

  /**
   * Computes the derivative with respect to the standard deviation
   * from the second derivative with respect to the state variable.
   *
   * @param rGammaToVega Before \p vega this parameter represents
   * the second derivative with respect to state variable. After \p
   * vega it is replaced with the first derivative with respect to
   * the standard deviation.
   */
  void vega (std::valarray<double> &rGammaToVega) const;

private:
  std::shared_ptr<IGaussRollback> m_uP;
  double m_dH, m_dVar;
  unsigned m_iSize;
};

/**
 * @brief Implementations of the operator of conditional expectations
 * with respect to gaussian distribution.
 *
 * @see IGaussRollback and GaussRollback
 */
namespace NGaussRollback
{
/**
 * Explicit finite-difference scheme.
 *
 * @param dP Equals \f$ \tau/(2h^2)\f$, where \f$\tau\f$ is the
 * time step and \f$h\f$ is the state step.
 * @return cfl::GaussRollback
 */
cfl::GaussRollback expl (double dP = 1. / 3.);

/**
 * Fully implicit finite-difference scheme.
 *
 * @param dP Equals \f$ \tau/(2h^2)\f$, where \f$\tau\f$ is the
 * time step and \f$h\f$ is the state step.
 * @return cfl::GaussRollback
 */
cfl::GaussRollback impl (double dP = 1.);

/**
 * Crank-Nicolson (symmetric) finite-difference scheme.
 *
 * @param dR Equals \f$ \tau/h\f$, where \f$\tau\f$ is the time
 * step and \f$h\f$ is the state step.
 * @return cfl::GaussRollback
 */
cfl::GaussRollback crankNicolson (double dR = 1.);

/**
 * Computation of gaussian conditional expectation with radix-2
 * FFT.  The grid needs to have \f$2^n\f$ nodes.
 *
 * @return cfl::GaussRollback
 */
cfl::GaussRollback fft2 ();

/**
 * Computation of gaussian conditional expectation with general
 * FFT. The algorithm runs efficiently if the numbers of nodes is
 * a product of 2,3, and 5.
 *
 * @return cfl::GaussRollback
 */
cfl::GaussRollback fft ();

/**
 * A wrapper of "fast" scheme with explicit and implicit schemes to
 * improve the performance.
 *
 * @param iExplSteps The number of time steps of explicit scheme
 * used at the beginning to smooth boundary conditions.
 * @param rFast The main (fast) finite-difference scheme.
 * @param iImplSteps The number of time steps of fully implicit
 * scheme used at the end to filter round-off errors.
 * @param dExplP Equals \f$ \tau/(2h^2)\f$, where \f$\tau\f$ is
 * the time step in the explicit scheme and \f$h\f$ is the state
 * step.
 * @param dImplP Equals \f$ \tau/(2h^2)\f$, where \f$\tau\f$ is
 * the time step in the implicit scheme and \f$h\f$ is the state
 * step.
 * @return cfl::GaussRollback
 */
cfl::GaussRollback chain (unsigned iExplSteps, const cfl::GaussRollback &rFast,
                          unsigned iImplSteps, double dExplP = 1. / 3.,
                          double dImplP = 1.);

/**
 * A number of default choices of 3-layer or chain schemes parameterized by
 * fast schemes. The numbers of explicit and implicit steps are chosen so that
 * the time spend in every layer (explicit, fast, and implicit) is roughly the
 * same.
 *
 * @param sFastScheme The name of the fast scheme in the implementation of the
 * 3-layer scheme. The possible choices are
 * - <code> "crankNicolson" </code> for Crank-Nicolson scheme.
 * - <code> "fft2" </code> for Fast Fourier Transform with radix 2.
 * - <code> "fft" </code> for Fast Fourier Transform with general radix.
 *
 * @return cfl::GaussRollback
 */
cfl::GaussRollback chain (const char *sFastScheme = "fft2");
} // namespace NGaussRollback
/** @} */
} // namespace cfl

#include "cfl/Inline/iGaussRollback.hpp"
#endif // of __cflGaussRollback_hpp__
