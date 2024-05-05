#ifndef __cflError_hpp__
#define __cflError_hpp__

/**
 * @file Error.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Exceptions in cfl library.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/Macros.hpp"
#include <exception>
#include <string>

namespace cfl
{
/**
 * @defgroup cflMisc Auxiliary items.
 *
 * This module contains miscellaneous items related to the library.
 */

/**
 * @ingroup cflMisc
 *
 * @defgroup cflError Exceptions.
 *
 * This module contains exception classes and functions for cfl.
 * @{
 */

/**
 * @brief  Exception class for cfl.
 *
 * This is the exception class for cfl.
 *
 * @see NError
 */
class Error : public std::exception
{
public:
  /**
   * The constructor.
   *
   * @param pWhere The information on an error.
   */
  Error (const char *pWhere);

  /**
   * The function returns information on the thrown exception.
   *
   * @return The description of the error.
   */
  const char *what () const throw ();

private:
  std::string m_sErr;
};

/**
 * @brief  Implementations of Error class for different types of exceptions.
 *
 * This namespace contains implementations of Error class adapted for
 * different types of exceptions.
 *
 * @see Error
 */
namespace NError
{
/**
 * This type of error indicates that some parameter is out of acceptable
 * range. For example, negative volatility, invalid time.
 *
 * @param pWhere Additional info on the error.
 * @return Generic class for exceptions in cfl.
 */
Error range (const char *pWhere);

/**
 * This type of error usually indicates that an input vector (for example, a
 * vector of event times) is not sorted, while it should be.
 *
 * @param pWhere An additional info on the error.
 * @return The Error class for exceptions in cfl.
 */
Error sort (const char *pWhere);

/**
 * This type of error indicates that some vector or array does not have
 * a correct size.
 *
 * @param pWhere An additional info on the error. For example, which vector
 * has invalid size.
 * @return The Error class for exceptions in cfl.
 */
Error size (const char *pWhere);
} // namespace NError
/** @} */
} // namespace cfl

#include "cfl/Inline/iError.hpp"
#endif //__cflError_hpp__
