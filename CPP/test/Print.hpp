#ifndef __test_all_Print_hpp__
#define __test_all_Print_hpp__

/**
 * @file Print.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Collections of testing functions.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/Data.hpp"
#include "cfl/Function.hpp"
#include "cfl/Slice.hpp"
#include "test/Index.hpp"
#include "test/Output.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <valarray>
#include <vector>

namespace test
{
/**
 * @ingroup test_all
 *
 * @{
 */

/**
 * Returns uniform partition of an interval.
 *
 * @param dL The left point of the interval.
 * @param dR The right point of the interval.
 * @param iN The number of points in the partition.
 * @return std::valarray<double> The uniform partition
 * of the interval \f$[L,U]\f$ on \p iN points.
 */
std::valarray<double> getArg (double dL, double dR, unsigned iN);

/**
 * Returns a testing vector of event times. The first element is greater than
 * \p dInitialTime.
 *
 * @param dInitialTime The initial time. It is not included in the result.
 * @param dMaturity The maturity.
 * @param iN The number of event times.
 * @return The testing vector of event times on
 * <code> [dInitialTime, dMaturity] </code> with \p iN elements.
 * \p dInitialTime is not included in the result.
 */
std::vector<double> getTimes (double dInitialTime, double dMaturity,
                              unsigned iN);

/**
 * Returns uniformly distributed random points.
 *
 * @param dL The left point of the interval.
 * @param dR The right point of the interval.
 * @param iN The number of random points.
 * @return std::valarray<double> The partition of the interval
 * \f$[L,U]\f$ on \p iN points having the uniform distribution.
 */
std::valarray<double> getRandArg (double dL, double dR, unsigned iN);

/**
 * Get array of values of a function.
 *
 * @param rF The function.
 * @param rArg The vector of arguments.
 * @return std::valarray<double> The array of the values of
 * the function \p rF at the arguments \p rArg.
 */
std::valarray<double> getValues (const cfl::Function &rF,
                                 const std::valarray<double> &rArg);

/**
 * Prints risk report for an option.
 *
 * @param rOption The values of the option as a function of a state process.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 * @param dFactor The delta-gamma factor.
 * @param dShift The state step.
 */
void printRisk (const cfl::Function &rOption, double dRelErr, double dAbsErr,
                double dFactor = 20., double dShift = 0.01);

/**
 * Prints the parameters of a regular cash flow.
 *
 * @param rCashFlow The parameters of a regular cash flow.
 * @param rName The name of the cash flow.
 */
void printCashFlow (const cfl::Data::CashFlow &rCashFlow,
                    const std::string &rName);

/**
 * Prints the parameters of a swap.
 *
 * @param rSwap The parameters of a swap.
 * @param rName The name of the swap.
 */
void printSwap (const cfl::Data::Swap &rSwap, const std::string &rName);

/**
 * Compares numerical and exact results.
 *
 * @param rExact The exact slice.
 * @param rApprox The approximate slice.
 * @param rTitle The title of the table.
 * @param iColumn The size of every column.
 * @param iSpace The space between columns.
 * @param iMaxRows The maximal number of rows.
 */
void compare (const std::valarray<double> &rExact,
              const std::valarray<double> &rApprox, const std::string &rTitle,
              unsigned iColumn = 10, unsigned iSpace = 10,
              unsigned iMaxRows = 9);

/**
 * Prints displayed message.
 *
 * @param sMessage The message.
 * @param bExtraLine If \p true, then extra empty line is added.
 */
void print (const std::string &sMessage, bool bExtraLine = true);

/**
 * Prints the value of a parameter.
 *
 * @param dValue The value of the parameter.
 * @param rName The name of the parameter.
 * @param bExtraLine If \p true, then extra line is added at the end.
 */
void print (double dValue, const std::string &rName, bool bExtraLine = false);

/**
 * Prints the values of an array.
 *
 * @param start The iterator to the first element.
 * @param end The iterator to the last plus one element.
 * @param rName The name of the array.
 */
template <class T> void print (T start, T end, const std::string &rName);

/**
 * Prints the subset of the table of the results.
 *
 * @param rValues The vector of columns of the results.
 * @param rNames The vector of titles of the columns.
 * @param sMessage The title of the table.
 * @param rColumnSize The vector of the widths of the columns.
 * @param iSpace The space between the columns.
 * @param iMaxRows The maximal number of rows in the middle of total table
 * to be printed.
 */
void printTable (const std::vector<std::valarray<double> > &rValues,
                 const std::vector<std::string> &rNames,
                 const std::string &sMessage,
                 const std::vector<unsigned> &rColumnSize,
                 unsigned iSpace = 10, unsigned iMaxRows = 20);

/**
 * Prints the subset of the table of the results.
 *
 * @param rValues The vector of columns of the results.
 * @param rNames The vector of titles of the columns.
 * @param sMessage The title of the table.
 * @param iColumn The width of the column.
 * @param iSpace The space between the columns.
 * @param iMaxRows The maximal number of rows in the middle of total table
 * to be printed.
 */
void printTable (const std::vector<std::valarray<double> > &rValues,
                 const std::vector<std::string> &rNames,
                 const std::string &sMessage, unsigned iColumn = 10,
                 unsigned iSpace = 10, unsigned iMaxRows = 20);

/**
 * Prints the subset of the table of the results.
 *
 * @param rValues The vector of columns of the results.
 * @param rNames The vector of titles of the columns.
 * @param sMessage The title of the table.
 * @param iColumn The width of the column.
 * @param iSpace The space between the columns.
 * @param iMaxRows The maximal number of rows in the middle of total table
 * to be printed.
 */
void printTable (const std::vector<std::vector<double> > &rValues,
                 const std::vector<std::string> &rNames,
                 const std::string &sMessage, unsigned iColumn = 10,
                 unsigned iSpace = 10, unsigned iMaxRows = 20);

/**
 * Prints the table of the results.
 *
 * @param rF The vector of functions.
 * @param rNames The vector of titles of the columns.
 * @param rArg The vector of arguments for the functions.
 * @param sMessage The title of the table.
 * @param iColumn The width of the columns for the values of the
 * functions.
 * @param iArg The width of the column for the arguments.
 * @param iSpace The space between the columns.
 * @param sArg The title of the column for arguments.
 */
void printTable (const std::vector<cfl::Function> &rF,
                 const std::vector<std::string> &rNames,
                 const std::valarray<double> &rArg,
                 const std::string &sMessage, unsigned iColumn = 10,
                 unsigned iArg = 10, unsigned iSpace = 10,
                 const std::string &sArg = "arg");

/**
 * Prints the sums of squares of estimated and actual errors.
 *
 * @param rEstErr Estimated error function.
 * @param rActErr Actual error function.
 * @param rArg The vector of arguments.
 */
void printChi2 (const cfl::Function &rEstErr, const cfl::Function &rActErr,
                const std::valarray<double> &rArg);

/**
 * Prints the values of the function at given arguments.
 *
 * @param rF The function.
 * @param rArg The vector of arguments.
 * @param rTitle The title of the column.
 */
void printValues (const cfl::Function &rF, const std::valarray<double> &rArg,
                  const std::string &rTitle);
/**
 * Returns the rounding function.
 *
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 * @return std::function<double(double)>
 */
std::function<double (double)> roundResult (double dRelErr = 1e-12,
                                            double dAbsErr = 1e-12);

/**
 * Prints values of option on a single stock.
 *
 * @param rOption The dependence of option on the relative change of stock
 * price.
 * @param dSpot The initial stock price.
 * @param dInterval The interval of initial values for stock price.
 * @param iPoints The number of points to print.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void reportAssetModel (const cfl::Function &rOption, double dSpot,
                       double dInterval, unsigned iPoints, double dRelErr,
                       double dAbsErr);

/**
 * Prints values of interest rate option.
 *
 * @param rOption The dependence of option on the change of interest rate.
 * @param dShortRate The initial short rate.
 * @param dInterval The interval of initial values for short rate.
 * @param iPoints The number of points to print.
 * @param dRelErr The relative error.
 * @param dAbsErr The absolute error.
 */
void reportInterestRateModel (const cfl::Function &rOption, double dShortRate,
                              double dInterval, unsigned iPoints,
                              double dRelErr, double dAbsErr);
/** @} */
} // namespace test

#include "test/Inline/iPrint.hpp"
#endif // of __test_all_Print_hpp__
