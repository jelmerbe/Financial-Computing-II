#ifndef __cflMacros_hpp__
#define __cflMacros_hpp__

/**
 * @file Macros.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Macros for cfl library.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <assert.h>

/**
 * @ingroup cflMisc
 *
 * @defgroup cflMacros Macros and constants.
 *
 * This module contains macros and constants for cfl library.
 * @{
 */

/**
 * Checks the validity of input parameters.
 *
 */
#define PRECONDITION assert

/**
 * Checks the validity of parameters in the middle
 * of an implementation.
 *
 */
#define ASSERT assert

/**
 * Checks the validity of output parameters.
 *
 */
#define POSTCONDITION assert
/** @} */

/**
 * @brief Main namespace for cfl library.
 */
namespace cfl
{
/**
 * @ingroup cflMacros
 * @{
 *
 * The constant for a tiny but nonzero positive quantity like one
 * millisecond in a year or a smallest safe divider: 1E-10
 * \f$\approx\f$ 3 milliseconds.
 *
 */
const double EPS = 1E-10;

/**
 * The constant for the smallest difference between the event times as
 * year fraction: 1E-5 \f$\approx\f$ 5 minutes.
 *
 */
const double TIME_EPS = 1E-5;

/**
 * The constant for the smallest possible variance.
 *
 */
const double VAR_EPS = 1E-12;

/**
 * The constant for a very large positive real number.
 *
 */
const double OMEGA = 1E20;

/**
 * The constant for the maximal number of iterations in numerical routines
 * such as root searching.
 *
 */
const unsigned IMAX = 1000;
/** @} */
} // namespace cfl

#endif //__cflMacros_hpp__
