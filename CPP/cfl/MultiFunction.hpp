#ifndef __cflMultiFunction_hpp__
#define __cflMultiFunction_hpp__

/**
 * @file MultiFunction.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Multi-dimensional function object.
 * @version 1.0
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cfl/Function.hpp"
#include "cfl/Macros.hpp"
#include <cmath>
#include <valarray>
#include <vector>

namespace cfl
{
/**
 * @ingroup cflFunctionObjects
 *
 * @defgroup cflMultiFunction Multi-dimensional function object.
 *
 * This module deals with multifunction object.
 * @{
 */

/**
 * @brief The interface for a multifunction.
 *
 * The abstract class for a multifunction. Its implementation is used
 * to construct concrete class MultiFunction.
 *
 * @see MultiFunction
 */
class IMultiFunction
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IMultiFunction (){};

  /**
   * Returns the value of the multifunction for the given argument.
   *
   * @param rX The argument of the multifunction. The size of \p rX
   * equals the dimension of the domain.
   * @return The value of the multifunction at \p rX. The size of the
   * return array equals the dimension of the range.
   */
  virtual std::valarray<double>
  operator() (const std::valarray<double> &rX) const = 0;

  /**
   * Returns the values of the functions with given indices.
   *
   * @param rX The argument of the multifunction. The size of \p rX
   * equals the dimension of the domain.
   * @param rIndices The indices of the functions to return.
   * @return The array of the values of the functions with indices \p
   * rIndices at \p rX. The size of the return array equals the size
   * of \p rIndices.
   */
  virtual std::valarray<double>
  operator() (const std::valarray<double> &rX,
              const std::valarray<std::size_t> &rIndices) const = 0;

  /**
   * Tests whether the argument belongs to the domain of the
   * multifunction.
   *
   * @param rX The argument of the multifunction. The size of \p rX
   * equals the dimension of the domain.
   * @return Returns \p true if \p rX belongs to the domain of the
   * multifunction and returns \p false otherwise.
   */
  virtual bool belongs (const std::valarray<double> &rX) const = 0;

  /**
   * The dimension of the domain of the multifunction.
   *
   * @return The dimension of the domain of the multifunction.
   */
  virtual unsigned dimD () const = 0;

  /**
   * The dimension of the range of the multifunction.
   *
   * @return The dimension of the range of the multifunction.
   */
  virtual unsigned dimR () const = 0;
};

/**
 * @brief  The concrete class for a multifunction.
 *
 * The standard class for a multifunction. It is constructed from a
 * new implementation of IMultiFunction.
 *
 * @see IMultiFunction
 */
class MultiFunction
{
public:
  /**
   * Constructs \p *this from a new implementation of
   * IMultiFunction. A copy of the new implementation is kept in \p
   * *this. This is the main constructor of \p MultiFunction.
   *
   * @param pNewF The pointer to a new implementation of IMultiFunction.
   */
  explicit MultiFunction (IMultiFunction *pNewF);

  /**
   * Constructs constant multifunction with value \p rV on the space
   * with dimension \p iDimD. The dimension of the range is given by
   * the size of \p rV.
   *
   * @param rV The value of the multifunction.
   * @param iDimD The dimension of the domain.
   */
  explicit MultiFunction (const std::valarray<double> &rV
                          = std::valarray<double> (0., 1),
                          unsigned iDimD = 1);

  /**
   * Constructs \p MultiFunction whose range is a subset of the input
   * multifunction.
   *
   * @param rF The constant reference to the input MultiFunction.
   * A copy of \p rF is created inside of \p *this.
   * @param rIndices The array of indices of input functions to keep.
   */
  MultiFunction (const MultiFunction &rF,
                 const std::valarray<std::size_t> &rIndices);

  /**
   * Constructs \p MultiFunction from \p Function. The domain and the
   * range have dimensions one.
   *
   * @param rF The constant reference to the input \p Function. A copy
   * of \p rF is kept inside of \p *this.
   */
  MultiFunction (const Function &rF);

  /**
   * Constructs \p MultiFunction from \p std::function objects.  The
   * domain is the space of dimension \p iDimD.  The range is the
   * space of dimension \p iDimR.
   *
   * @param rFF The constant reference to \p std::function describing
   * the constrained functional operator.  A copy of \p rFF is created
   * inside of \p *this.
   * @param rF The constant reference to \p std::function describing
   * the standard functional operator.  A copy of \p rF is created
   * inside of \p *this.
   * @param iDimD The dimension of the domain.
   * @param iDimR The dimension of the range.
   */
  MultiFunction (const std::function<std::valarray<double> (
                     const std::valarray<double> &,
                     const std::valarray<std::size_t> &)> &rFF,
                 const std::function<std::valarray<double> (
                     const std::valarray<double> &)> &rF,
                 unsigned iDimD, unsigned iDimR);

  /**
   * Constructs \p MultiFunction from \p std::function objects.
   *
   * @param rFF The constant reference to \p std::function describing
   * the constrained functional operator.  A copy of \p rFF is
   * created inside of \p *this.
   * @param rF The constant reference to \p std::function describing
   * the standard functional operator.  A copy of \p rF is created
   * inside of \p *this.
   * @param rBelongs The constant reference to the domain descriptor.
   * A copy of \p rBelongs is created inside of \p *this.
   * @param iDimD The dimension of the domain.
   * @param iDimR The dimension of the range.
   */
  MultiFunction (
      const std::function<std::valarray<double> (
          const std::valarray<double> &, const std::valarray<std::size_t> &)>
          &rFF,
      const std::function<
          std::valarray<double> (const std::valarray<double> &)> &rF,
      const std::function<bool (const std::valarray<double> &)> &rBelongs,
      unsigned iDimD, unsigned iDimR);

  /**
   * Returns the value of the multifunction for a given argument.
   *
   * @param rX The argument of the multifunction. The size of \p rX
   * equals the dimension of the domain.
   * @return The value of \p *this at \p rX. The size of the return
   * array equals the dimension of the range.
   */
  std::valarray<double> operator() (const std::valarray<double> &rX) const;

  /**
   * Returns the values of the functions with given indices.
   *
   * @param rX The argument of the multifunction. The size of \p rX
   * equals the dimension of the domain.
   * @param rIndices The indices of the functions to return.
   * @return The values of the functions with indices \p rIndices at
   * \p rX. The size of the return array equals the size of \p
   * rIndices.
   */
  std::valarray<double>
  operator() (const std::valarray<double> &rX,
              const std::valarray<std::size_t> &rIndices) const;

  /**
   * @copydoc IMultiFunction::belongs
   */
  bool belongs (const std::valarray<double> &rX) const;

  /**
   * @copydoc IMultiFunction::dimD
   */
  unsigned dimD () const;

  /**
   * @copydoc IMultiFunction::dimR
   */
  unsigned dimR () const;

  /**
   * Replaces \p *this with the sum of \p *this and \p rF.  The new
   * domain of \p *this is the intersection of its old domain with the
   * domain of \p rF.  A copy of \p rF is created inside of \p *this.
   *
   * @param rF Constant reference to the addend. It has the same
   * dimensions as \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator+= (const MultiFunction &rF);

  /**
   * Replaces \p *this with the product of \p *this and \p rF.  The
   * new domain of \p *this is the intersection of its old domain with
   * the domain of \p rF.  A copy of \p rF is created inside of \p
   * *this.
   *
   * @param rF Constant reference to the multiplier. It has the same
   * dimensions as \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator*= (const MultiFunction &rF);

  /**
   * Replaces \p *this with the difference between \p *this and \p rF.
   * The new domain of \p *this is the intersection of its old domain
   * with the domain of \p rF.  A copy of \p rF is created inside of
   * \p *this.
   *
   * @param rF Constant reference to the subtrahend.  It has the same
   * dimensions as \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator-= (const MultiFunction &rF);

  /**
   * Replaces \p *this with the ratio of \p *this and \p rF.  The new
   * domain of \p *this is the intersection of its old domain with the
   * domain of \p rF.  A copy of \p rF is created inside of \p *this.
   *
   * @param rF Constant reference to the divisor. It has the same
   * dimensions as \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator/= (const MultiFunction &rF);

  /**
   * Replaces \p *this with the sum of \p *this and \p rV. A copy of
   * \p rV is created inside of \p *this.
   *
   * @param rV The vector to be added to the function. It has the same
   * size as the dimension of the range of \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator+= (const std::valarray<double> &rV);

  /**
   * Replaces \p *this with the difference between \p *this and \p
   * rV. A copy of \p rV is created inside of \p *this.
   *
   * @param rV The vector to be subtracted from the function. It has
   * the same size as the dimension of the range of \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator-= (const std::valarray<double> &rV);

  /**
   * Replaces \p *this with the product of \p *this and \p rV. A copy of
   * \p rV is created inside of \p *this.
   *
   * @param rV The vector to be multiplied by the function. It has the
   * same size as the dimension of the range of \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator*= (const std::valarray<double> &rV);

  /**
   * Replaces \p *this with the ratio of \p *this and \p rV. A copy of
   * \p rV is created inside of \p *this.
   *
   * @param rV The divisor vector. It has the same size as the
   * dimension of the range of \p *this.
   * @return Reference to \p *this.
   */
  MultiFunction &operator/= (const std::valarray<double> &rV);

  /**
   * Replaces \p *this with the sum of \p *this and \p dV.
   *
   * @param dV The number to be added to the function.
   * @return Reference to \p *this.
   */
  MultiFunction &operator+= (double dV);

  /**
   * Replaces \p *this with the difference between \p *this and \p
   * dV.
   *
   * @param dV The number to be subtracted from the function.
   * @return Reference to \p *this.
   */
  MultiFunction &operator-= (double dV);

  /**
   * Replaces \p *this with the product of \p *this and \p dV.
   *
   * @param dV The number to be multiplied by the function.
   * @return Reference to \p *this.
   */
  MultiFunction &operator*= (double dV);

  /**
   * Replaces \p *this with the ratio of \p *this and \p dV.
   *
   * @param dV The divisor number.
   * @return Reference to \p *this.
   */
  MultiFunction &operator/= (double dV);

private:
  std::shared_ptr<IMultiFunction> m_pF;
};

/**
 * Returns the composition multifunction \p rOp(rF). The operator \p
 * rOp is applied componentwise to the values of \p rF.  The result
 * contains copies of \p rF and \p rOp. The composition multifunction
 * \p rOp(rF) has the same domain and the same range dimension as \p
 * rF.
 *
 * @param rF The input multifunction.
 * @param rOp The unary operator applied componentwise to the values
 * of \p rF.
 * @return The composition multifunction \p rOp(rF).
 */
MultiFunction apply (
    const MultiFunction &rF,
    const std::function<std::valarray<double> (const std::valarray<double> &)>
        &rOp);

/**
 * Returns the composition multifunction \p rOp(rF,rG). The result
 * contains copies of \p rF, \p rG, and \p rOp. The domain of the
 * result is the intersection of the domains of \p rF and \p rG.  The
 * operator \p rOp is applied componentwise to the values of \p rF and
 * \p rG. The range dimensions of \p rOp(rF,rG), \p rF, and \p rG are
 * the same.
 *
 * @param rF The first input multifunction.
 * @param rG The second input multifunction.
 * @param rOp The binary operator applied componentwise to the values
 * of \p rF and \p rG.
 * @return The composition multifunction \p rOp(rF,rG).
 */
MultiFunction apply (
    const MultiFunction &rF, const MultiFunction &rG,
    const std::function<std::valarray<double> (
        const std::valarray<double> &, const std::valarray<double> &)> &rOp);

/**
 * Returns minus \p rF. The result contains copy of \p rF.
 *
 * @param rF The multifunction whose minus is computed.
 * @return The multifunction given by <code>-rF</code>.
 */
MultiFunction operator- (const MultiFunction &rF);

/**
 * Returns the sum of \p rF and \p rG.  The result contains copies of
 * \p rF and \p rG.  The domain of the result is the intersection of
 * the domains of \p rF and \p rG. Its range has the same dimension as
 * the ranges of \p rF and \p rG.
 *
 * @param rF The first addend multifunction.
 * @param rG The second addend multifunction.
 * @return The multifunction given by <code>rF+rG</code>.
 */
MultiFunction operator+ (const MultiFunction &rF, const MultiFunction &rG);

/**
 * Returns the sum of \p rF and \p rV.  The result contains copies of
 * \p rF and \p rV.  The domain of the result is that of \p rF.  Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rF The multifunction-term of the sum.
 * @param rV The array-term of the sum.
 * @return The multifunction given by <code>rF+rV</code>.
 */
MultiFunction operator+ (const MultiFunction &rF,
                         const std::valarray<double> &rV);

/**
 * Returns the sum of \p rV and \p rF. The result contains copies of
 * \p rV and \p rF.  The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rV The array-term of the sum.
 * @param rF The multifunction-term of the sum.
 * @return The multifunction given by <code>rV+rF</code>
 */
MultiFunction operator+ (const std::valarray<double> &rV,
                         const MultiFunction &rF);

/**
 * Returns the sum of \p rF and \p dV.  The result contains a copy of
 * \p rF.  The domain of the result is that of \p rF. Its range has
 * the same dimension as the range of \p rF.
 *
 * @param rF The multifunction-term of the sum.
 * @param dV The number-term of the sum.
 * @return The multifunction given by <code>rF+dV</code>.
 */
MultiFunction operator+ (const MultiFunction &rF, double dV);

/**
 * Returns the sum of \p dV and \p rF. The result contains a copy of
 * \p rF.  The domain of the result is that of \p rF. Its range has
 * the same dimension as the range of \p rF.
 *
 * @param dV The number-term of the sum.
 * @param rF The multifunction-term of the sum.
 * @return The multifunction given by <code>dV+rF</code>
 */
MultiFunction operator+ (double dV, const MultiFunction &rF);

/**
 * Returns the difference between \p rF and \p rG.  The result
 * contains copies of \p rF and \p rG.  The domain of the result is
 * the intersection of the domains of \p rF and \p rG. Its range has
 * the same dimension as the ranges of \p rF and \p rG.
 *
 * @param rF The minuend multifunction.
 * @param rG The subtrahend multifunction.
 * @return The multifunction given by <code>rF-rG</code>
 */
MultiFunction operator- (const MultiFunction &rF, const MultiFunction &rG);

/**
 * Returns the difference between \p rV and \p rF. The result contains
 * copies of \p rV and \p rF. The domain of the result is that of \p
 * rF. Its range has the same dimension as the range of \p rF and
 * equals the size of \p rV.
 *
 * @param rV The minuend array.
 * @param rF The subtrahend multifunction.
 * @return The multifunction given by <code>rV-rF</code>
 */
MultiFunction operator- (const std::valarray<double> &rV,
                         const MultiFunction &rF);

/**
 * Returns the difference between \p rF and \p rV.  The result
 * contains copies of \p rF and \p rV.  The domain of the result is
 * that of \p rF. Its range has the same dimension as the range of \p
 * rF and equals the size of \p rV.
 *
 * @param rF The minuend multifunction.
 * @param rV The subtrahend array.
 * @return The multifunction given by <code>rF-rV</code>.
 */
MultiFunction operator- (const MultiFunction &rF,
                         const std::valarray<double> &rV);

/**
 * Returns the difference between \p dV and \p rF. The result contains
 * a copy of \p rF. The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF.
 *
 * @param dV The minuend number.
 * @param rF The subtrahend multifunction.
 * @return The multifunction given by <code>dV-rF</code>
 */
MultiFunction operator- (double dV, const MultiFunction &rF);

/**
 * Returns the difference between \p rF and \p dV.  The result
 * contains a copy of \p rF.  The domain of the result is that of \p
 * rF. Its range has the same dimension as the range of \p rF.
 *
 * @param rF The minuend multifunction.
 * @param dV The subtrahend number.
 * @return The multifunction given by <code>rF-dV</code>.
 */
MultiFunction operator- (const MultiFunction &rF, double dV);

/**
 * Returns the product of \p rF and \p rG.  The result contains
 * copies of \p rF and \p rG.  The domain of the result is the
 * intersection of the domains of \p rF and \p rG. Its range has
 * the same dimension as the ranges of \p rF and \p rG.
 *
 * @param rF The first multiplier multifunction.
 * @param rG The second multiplier multifunction.
 * @return The multifunction given by <code>rF*rG</code>
 */
MultiFunction operator* (const MultiFunction &rF, const MultiFunction &rG);

/**
 * Returns the product of \p rF and \p rV.  The result contains copies
 * of \p rF and \p rV.  The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rF The multiplier multifunction.
 * @param rV The multiplier array.
 * @return The multifunction given by <code>rF*rV</code>.
 */
MultiFunction operator* (const MultiFunction &rF,
                         const std::valarray<double> &rV);

/**
 * Returns the product of \p rV and \p rF. The result contains copies
 * of \p rV of \p rF.  The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rV The multiplier array.
 * @param rF The multiplier multifunction.
 * @return The multifunction given by <code>rV*rF</code>
 */
MultiFunction operator* (const std::valarray<double> &rV,
                         const MultiFunction &rF);

/**
 * Returns the product of \p rF and \p dV.  The result contains a copy
 * of \p rF.  The domain of the result is that of \p rF. Its range has
 * the same dimension as the range of \p rF.
 *
 * @param rF The multiplier multifunction.
 * @param dV The multiplier number.
 * @return The multifunction given by <code>rF*dV</code>.
 */
MultiFunction operator* (const MultiFunction &rF, double dV);

/**
 * Returns the product of \p dV and \p rF. The result contains a copy
 * of \p rF.  The domain of the result is that of \p rF.  Its range
 * has the same dimension as the range of \p rF.
 *
 * @param dV The multiplier number.
 * @param rF The multiplier multifunction.
 * @return The multifunction given by <code>dV*rF</code>
 */
MultiFunction operator* (double dV, const MultiFunction &rF);

/**
 * Returns the ratio of \p rF and \p rG.  The result contains
 * copies of \p rF and \p rG.  The domain of the result is the
 * intersection of the domains of \p rF and \p rG. Its range has
 * the same dimension as the ranges of \p rF and \p rG.
 *
 * @param rF The dividend multifunction.
 * @param rG The divisor multifunction.
 * @return The multifunction given by <code>rF/rG</code>
 */
MultiFunction operator/ (const MultiFunction &rF, const MultiFunction &rG);

/**
 * Returns the ratio of \p rF and \p rV.  The result contains copies
 * of \p rF and \p rV.  The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rF The dividend multifunction.
 * @param rV The divisor array.
 * @return The multifunction given by <code>rF/rV</code>.
 */
MultiFunction operator/ (const MultiFunction &rF,
                         const std::valarray<double> &rV);

/**
 * Returns the ratio of \p rV and \p rF. The result contains copies
 * of \p rV and \p rF.  The domain of the result is that of \p rF. Its
 * range has the same dimension as the range of \p rF and equals the
 * size of \p rV.
 *
 * @param rV The dividend array.
 * @param rF The divisor multifunction.
 * @return The multifunction given by <code>rV/rF</code>
 */
MultiFunction operator/ (const std::valarray<double> &rV,
                         const MultiFunction &rF);

/**
 * Returns the ratio of \p rF and \p dV.  The result contains a copy
 * of \p rF. The domain of the result is that of \p rF. Its range has
 * the same dimension as the range of \p rF.
 *
 * @param rF The dividend multifunction.
 * @param dV The divisor number.
 * @return The multifunction given by <code>rF/dV</code>.
 */
MultiFunction operator/ (const MultiFunction &rF, double dV);

/**
 * Returns the ratio of \p dV and \p rF. The result contains a copy of
 * \p rF.  The domain of the result is that of \p rF. Its range has
 * the same dimension as the range of \p rF.
 *
 * @param dV The dividend number.
 * @param rF The divisor multifunction.
 * @return The multifunction given by <code>dV/rF</code>
 */
MultiFunction operator/ (double dV, const MultiFunction &rF);

/**
 * Returns the absolute value of \p rF.  The result contains a
 * copy of \p rF.  The domain of the result is that of \p rF. Its range
 * has the same dimension as the range of \p rF.
 *
 * @param rF The multifunction whose absolute value is computed
 * componentwise.
 * @return The absolute value of \p rF.
 */
MultiFunction abs (const MultiFunction &rF);

/**
 * Returns the exponent of \p rF.  The result contains copy
 * of \p rF. The domain of the result is that of \p rF. Its range
 * has the same dimension as the range of \p rF.
 *
 * @param rF The multifunction whose exponent is computed componentwise.
 * @return The exponent of  \p rF.
 */
MultiFunction exp (const MultiFunction &rF);

/**
 * Returns the logarithm of \p rF.  The result contains copy
 * of \p rF. The domain of the result is that of \p rF. Its range
 * has the same dimension as the range of \p rF.
 *
 * @param rF The multifunction whose logarithm is computed componentwise.
 * @return The logarithm of  \p rF.
 */
MultiFunction log (const MultiFunction &rF);

/**
 * Returns the square root of \p rF.  The result contains the a
 * copy of \p rF. The domain of the result is that of \p rF. Its range
 * has the same dimension as the range of \p rF.
 *
 * @param rF The multifunction whose square root is computed componentwise.
 * @return The square root of  \p rF.
 */
MultiFunction sqrt (const MultiFunction &rF);

/**
 * Constrains multifunction to a domain of smaller dimension defined
 * by the multifunction \p rS.
 *
 * @param rF The input multifunction.
 * @param rS The section of the original domain. It maps a set of
 * dimension \p iDimD into the space of dimension \p rF.dimD().
 * @param rB The descriptor of the domain of \p rS.
 * @param iDimD The dimension of the domain of the result.
 * @return The restriction of the multifunction \p rF on the section
 * of the original domain defined by \p rS.
 */
MultiFunction section (
    const MultiFunction &rF,
    const std::function<std::valarray<double> (const std::valarray<double> &)>
        rS,
    const std::function<bool (const std::valarray<double> &)> rB,
    unsigned iDimD);

/**
 * Constrains multifunction to a domain of smaller dimension given by
 * the intersection of the old domain with the hyperplane keeping the
 * values of the fixed coordinates constant and equal \p rFixedArg.
 *
 * @param rF The input multifunction.
 * @param rFlexIndex The indices of the flexible coordinates. This
 * array is strictly increasing.
 * @param rFixedArg The values of the fixed coordinates.

 * @return The restriction of the multifunction \p rF on the points of
 * its domain where the coordinates with indices \p rFlexIndex are
 * flexible and the other coordinates are fixed and have the values \p
 * rFixedArg. We have
 * \code
 * rF.dimD() == rFlexIndex.size() + rFixedArg.size()
 * \endcode.
 */
MultiFunction section (const MultiFunction &rF,
                       const std::valarray<std::size_t> &rFlexIndex,
                       const std::valarray<double> &rFixedArg);

/**
 * Constructs tensor product of the vector of multifunctions.
 *
 * @param rF The input vector of multifunctions. They all have the same
 *  dimensions for the domain and range.

 * @return The tensor product of the vector of multifunctions. Its
 * domain the intersection of the domains of the input
 * multifunctions. Its range is the same as of the input
 * multifunctions.
 */
MultiFunction tensor (const std::vector<MultiFunction> &rF);

/** @} */
} // namespace cfl

#include "cfl/Inline/iMultiFunction.hpp"
#endif // of __cflMultiFunction_hpp__
