#ifndef __cflFunction_hpp__
#define __cflFunction_hpp__

/**
 * @file Function.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief One-dimensional function object.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/Error.hpp"
#include <cmath>
#include <functional>
#include <limits>
#include <memory>

namespace cfl
{
/**
 * @defgroup cflFunctionObjects Function objects.
 *
 * This module contains implementations of one- and multi-dimensional
 * function objects.
 */

/**
 * @ingroup cflFunctionObjects
 *
 * @defgroup cflFunction One-dimensional function object.
 *
 * This module deals with one-dimensional function object.
 * @{
 */

/**
 * @brief  Interface for one-dimensional function.
 *
 * Abstract class for one-dimensional function. Its
 * implementation  is used to construct concrete
 * class Function.
 *
 * @see Function
 */
class IFunction
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IFunction () {}

  /**
   * Returns the value of the function for a given argument.
   *
   * @param dX The argument.
   * @return  The value of the function at \p dX.
   */
  virtual double operator() (double dX) const = 0;

  /**
   * Tests whether an argument belongs to the domain of the function.
   *
   * @param dX The argument.
   * @return The function returns \p true if the argument \p dX belongs to its
   * domain and returns \p false otherwise.
   */
  virtual bool belongs (double dX) const = 0;
};

/**
 * @brief  The concrete class for a one-dimensional function.
 *
 * The standard class for a one-dimensional function.
 * It is constructed from a new implementation of IFunction.
 *
 * @see IFunction
 */
class Function
{
public:
  /**
   * Constructs constant function with value \p dV on the interval
   * [\p dL, \p dR].
   *
   * @param dV The value of the function.
   * @param dL The left point of the domain.
   * @param dR The right point of the domain.
   */
  explicit Function (double dV = 0., double dL = -OMEGA, double dR = OMEGA);

  /**
   * Constructs \p *this from a new implementation of IFunction. This is the
   * main constructor of Function.
   *
   * @param pNewP The pointer to a new implementation of IFunction.
   */
  explicit Function (IFunction *pNewP);

  /**
   * Constructs \p Function from an std::function.
   *
   * @param rF Constant reference to the function object.
   * A copy of \p rF is created inside of \p *this.
   * @param dL The left point of the domain.
   * @param dR The right point of the domain.
   */
  explicit Function (const std::function<double (double)> &rF,
                     double dL = -OMEGA, double dR = OMEGA);

  /**
   * Constructs \p Function from a pair of std::function objects.
   *
   * @param rF Constant reference to the function object.
   * A copy of \p rF is created inside of \p *this.
   * @param rBelongs Constant reference to the domain descriptor.
   * A copy of \p rBelongs is created inside of \p *this.
   */
  Function (const std::function<double (double)> &rF,
            const std::function<bool (double)> &rBelongs);

  /**
   * Assigns to \p *this the constant function with value \p dV.  The
   * domain becomes the real line.
   *
   * @param dV The value of the function.
   * @return Reference to \p *this.
   */
  Function &operator= (double dV);

  /**
   *  Returns the value of the function for a given argument.
   *
   * @param dX The argument.
   * @return The value of the function at \p dX.
   */
  double operator() (double dX) const;

  /**
   * @copydoc IFunction::belongs
   */
  bool belongs (double dX) const;

  /**
   * Replaces \p *this with the sum of \p *this and \p rF.  The new
   * domain of \p *this is the intersection of its old domain
   * with the domain of \p rF. A copy of \p rF is created
   * inside of \p *this.
   *
   * @param rF Constant reference to the addend.
   * @return Reference to \p *this.
   */
  Function &operator+= (const Function &rF);

  /**
   * Replaces \p *this with the product of \p *this and \p rF.  The
   * new domain of \p *this is the intersection of its old
   * domain with the domain of \p rF. A copy of \p rF is
   * created inside of \p *this.
   *
   * @param rF Constant reference to the multiplier.
   * @return Reference to \p *this.
   */
  Function &operator*= (const Function &rF);

  /**
   * Replaces \p *this with the difference between \p *this and \p
   * rF.  The new domain of \p *this is the intersection of its
   * old domain with the domain of \p rF. A copy of \p rF is
   * created inside of \p *this.
   *
   * @param rF Constant reference to the subtrahend.
   * @return Reference to \p *this.
   */
  Function &operator-= (const Function &rF);

  /**
   * Replaces \p *this with the ratio between \p *this and \p rF.
   * The new domain of \p *this is the intersection of its old
   * domain with the domain of \p rF. A copy of \p rF is
   * created inside of \p *this.
   *
   * @param rF The divisor function.
   * @return Reference to \p *this.
   */
  Function &operator/= (const Function &rF);

  /**
   * Replaces \p *this with the sum of \p *this and \p dV.
   *
   * @param dV The number to be added to the function.
   * @return Reference to \p *this.
   */
  Function &operator+= (double dV);

  /**
   * Replaces \p *this with the difference between \p *this and \p dV.
   *
   * @param dV The number to be subtracted from the function.
   * @return Reference to \p *this.
   */
  Function &operator-= (double dV);

  /**
   * Replaces \p *this with the product of \p *this and \p dV.
   *
   * @param dV The number to be multiplied by the function.
   * @return Reference to \p *this.
   */
  Function &operator*= (double dV);

  /**
   * Replaces \p *this with the ratio of \p *this and \p dV.
   *
   * @param dV The divisor number.
   * @return Reference to \p *this.
   */
  Function &operator/= (double dV);

private:
  std::shared_ptr<IFunction> m_pF;
};

/**
 * Returns the composition function \p rOp(rF).
 * The result contains copies of \p rF and \p rOp.
 *
 * @param rF A function.
 * @param rOp A unary operator.
 * @return The composition function \p rF(*this)
 */
Function apply (const Function &rF, const std::function<double (double)> &rOp);

/**
 * Returns the composition function \p rOp(rF,rG). The result contains
 * copies of \p rF,  \p rG, and \p rOp. The domain of the result is
 * the intersection of the domains of \p rF and \p rG.
 *
 * @param rF The first function.
 * @param rG The second function.
 * @param rOp A binary operator.
 * @return The composition function \p rOp(rF,rG).
 */
Function apply (const Function &rF, const Function &rG,
                const std::function<double (double, double)> &rOp);

/**
 * Returns minus \p rF. The result contains a copy of \p rF.
 * The domain of the result equals the domain of \p rF.
 *
 * @param rF A function object whose minus is computed.
 * @return The function given by <code>-rF</code>
 */
Function operator- (const Function &rF);

/**
 * Returns the sum of \p rF and \p rG.  The result contains
 * copies of \p rF and \p rG.  The domain of the result equals
 * the intersection of the domains of \p rF and \p rG.
 *
 * @param rF First addend function.
 * @param rG Second addend function.
 * @return The function given by <code>rF+rG</code>
 */
Function operator+ (const Function &rF, const Function &rG);

/**
 * Returns the sum of \p rF and \p dV. The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function-term of the sum.
 * @param dV The number-term of the sum.
 * @return The function given by <code>rF+dV</code>
 */
Function operator+ (const Function &rF, double dV);

/**
 * Returns the  sum of \p dV and \p rF. The result
 * contains a copy of \p rF.  The domain of the
 * result equals that of \p rF.
 *
 * @param dV The number-term of the sum.
 * @param rF The function-term of the sum.
 * @return The function given by <code>dV+rF</code>
 */
Function operator+ (double dV, const Function &rF);

/**
 * Returns the difference between \p rF and \p rG.  The result
 * contains copies of \p rF and \p rG.  The domain of the
 * result equals the intersection of the domains of \p rF and \p rG.
 *
 * @param rF The minuend function.
 * @param rG The subtrahend function.
 * @return The function given by <code>rF-rG</code>
 */
Function operator- (const Function &rF, const Function &rG);

/**
 * Returns the difference between \p rF and \p dV. The result
 * contains a copy of \p rF.  The domain of the result equals
 * that of \p rF.
 *
 * @param rF The minuend function.
 * @param dV The subtrahend function.
 * @return The function given by <code>rF-dV</code>
 */
Function operator- (const Function &rF, double dV);

/**
 * Returns the difference between \p dV and \p rF. The result
 * contains a copy of \p rF.  The domain of the result equals
 * that of \p rF.
 *
 * @param dV The minuend number.
 * @param rF The subtrahend function.
 * @return The function given by <code>dV-rF</code>
 */
Function operator- (double dV, const Function &rF);

/**
 * Returns the product of \p rF and \p rG.  The result contains
 * copies of \p rF and \p rG.  The domain of the result equals the
 * intersection of the domains of \p rF and \p rG.
 *
 * @param rF The first multiplier function.
 * @param rG The second multiplier function.
 * @return The function given by <code>rF*rG</code>
 */
Function operator* (const Function &rF, const Function &rG);

/**
 * Returns the product of \p rF and \p dV. The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The multiplier function.
 * @param dV The multiplier number.
 * @return The function given by <code>rF*dV</code>
 */
Function operator* (const Function &rF, double dV);

/**
 * Returns the product of \p dV and \p rF. The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param dV The multiplier number.
 * @param rF The multiplier function.
 * @return The function given by <code>dV*rF</code>
 */
Function operator* (double dV, const Function &rF);

/**
 * Returns the ratio of \p rF and \p rG.  The result contains
 * copies of \p rF and \p rG.  The domain of the result equals the
 * intersection of the domains of \p rF and \p rG.
 *
 * @param rF The dividend function.
 * @param rG The divisor function.
 * @return The function given by <code>rF/rG</code>
 */
Function operator/ (const Function &rF, const Function &rG);

/**
 * Returns the ratio of \p rF and \p dV. The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The dividend function.
 * @param dV The divisor number.
 * @return The function given by <code>rF/dV</code>
 */
Function operator/ (const Function &rF, double dV);

/**
 * Returns the ratio of \p dV and \p rF. The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param dV The dividend number.
 * @param rF The divisor function.
 * @return The function given by <code>dV/rF</code>
 */
Function operator/ (double dV, const Function &rF);

/**
 * Returns \p rF in the power \p dV. The result contains a copy
 * of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function-base.
 * @param dV The number-exponent.
 * @return The function \p rF in the power \p dV.
 */
Function pow (const Function &rF, double dV);

/**
 * Returns the absolute value of \p rF.  The result contains a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function whose absolute value is computed.
 * @return The absolute value of \p rF.
 */
Function abs (const Function &rF);

/**
 *  Returns the exponent of \p rF.  The result contains a copy
 * of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function whose exponent is computed.
 * @return The exponent of  \p rF.
 */
Function exp (const Function &rF);

/**
 * Returns the logarithm of \p rF.  The result contains a copy
 * of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function whose logarithm is computed.
 * @return The logarithm of  \p rF.
 */
Function log (const Function &rF);

/**
 * Returns the square root of \p rF.  The result contains the a
 * copy of \p rF.  The domain of the result equals that of \p rF.
 *
 * @param rF The function whose square root is computed.
 * @return The square root of  \p rF.
 */
Function sqrt (const Function &rF);

/** @} */
} // namespace cfl

#include "cfl/Inline/iFunction.hpp"
#endif // of __cflFunction_hpp__
