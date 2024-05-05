#ifndef __test_all_Main_hpp__
#define __test_all_Main_hpp__

/**
 * @file Main.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Collections of testing functions.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "test/Print.hpp"

/**
 * @brief Collection of testing functions.
 *
 */
namespace test
{
/**
 * @defgroup test_all Testing functions.
 *
 * A collection of useful testing functions.
 *
 * @{
 */

/**
 * Adapter of multifunction to function. The input multifunction needs to have
 * dimension one for the domain and range.
 *
 * @param rF The input multifunction. Its domain and range have dimension one.
 * @return cfl::Function
 */
cfl::Function toFunction (const cfl::MultiFunction &rF);

/**
 * The top program for running a test.
 *
 * @param rF The function that runs the test.
 * @param sProjectDir The name of the project directory relative to
 * the output directory.
 * @param sFile The name of the output file.
 * @param sTitle The name of the project. It is printed at the top
 * of the output file.
 */
void project (const std::function<void ()> &rF, const std::string &sProjectDir,
              const std::string &sFile, const std::string &sTitle);
/** @} */
} // namespace test

#endif // of __test_all_Main_hpp__
