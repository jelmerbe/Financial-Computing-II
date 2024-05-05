#ifndef __test_all_Data_hpp__
#define __test_all_Data_hpp__

/**
 * @file Data.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Testing of data functions.
 * @version 1.0
 * @date 2021-01-18
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "cfl/Fit.hpp"
#include "test/Main.hpp"
#include <string>

namespace test
{
/**
 * @brief Testing of data curves.
 *
 */
namespace Data
{
/**
 *
 * @defgroup test_all_Data Testing of data curves.
 *
 * This module contains testing functions for the
 * construction of input data curves for financial models.
 *
 * @{
 */

/**
 * Returns a pair of vectors of times and discount factors.
 *
 * @param dInitialTime The initial time
 * @return std::pair<std::vector<double>, std::vector<double> >
 */
std::pair<std::vector<double>, std::vector<double> >
getDiscount (double dInitialTime);

/**
 * Returns a discount curve with decreasing yield curve.
 *
 * @param dInitialTime The initial time
 * @return A discount curve.
 */
cfl::Function getDiscountCurve (double dInitialTime);

/**
 * Returns a pair of vectors of times and forward prices.
 *
 * @param dSpot The spot price.
 * @param dInitialTime The initial time.
 * @return std::pair<std::vector<double>, std::vector<double> >
 */
std::pair<std::vector<double>, std::vector<double> >
getForward (double dSpot, double dInitialTime);

/**
 * Returns the tuple: times, domestic discount factors,
 * and foreign discount factors.
 *
 * @param dSpot The spot fx rate.
 * @param dInitialTime The initial time.
 * @return std::tuple<std::vector<double>, std::vector<double>,
 * std::vector<double>>
 */
std::tuple<std::vector<double>, std::vector<double>, std::vector<double> >
getDiscountFX (double dSpot, double dInitialTime);

/**
 * Returns a pair of libor periods and rates.
 *
 * @param dInitialTime The initial time.
 * @return std::pair<std::vector<double>, std::vector<double>>
 */
std::pair<std::vector<double>, std::vector<double> >
getRate (double dInitialTime);

/**
 * Returns the vector of swap rates.
 *
 * @param dPeriod The period between payments.
 * @param dInitialTime The initial time.
 * @return std::vector<double>
 */
std::vector<double> getSwapRates (double dPeriod, double dInitialTime);

/**
 * Returns a pair of vectors. The first component stands
 * for swap rates. The second component stands for the number of periods.
 *
 * @param dPeriod The time inteval between payments
 * @param dInitialTime The initial time
 * @return std::pair<std::vector<double>, std::vector<unsigned>>
 */
std::pair<std::vector<double>, std::vector<unsigned> >
getSwapRatesPeriods (double dPeriod, double dInitialTime);

/**
 * Returns vectors of maturities and implied volatilities.
 *
 * @param dInitialTime The initial time.
 * @return std::pair<std::vector<double>, std::vector<double>>
 */
std::pair<std::vector<double>, std::vector<double> >
getVol (double dInitialTime);

/**
 * Returns maturities of options, maturities of underlying bonds,
 * and implied volatilities.
 *
 * @param dInitialTime The initial time.
 * @return std::tuple<std::vector<double>, std::vector<double>,
 * std::vector<double>>
 */
std::tuple<std::vector<double>, std::vector<double>, std::vector<double> >
getVolBonds (double dInitialTime);

/**
 * Returns fx fixed swap rates for domestic and foreign currencies.
 *
 * @param dSpotFX The spot fx rate.
 * @param dPeriod The time interval between payments.
 * @param dInitialTime The initial time.
 * @return std::pair<std::vector<double>, std::vector<double>>
 */
std::pair<std::vector<double>, std::vector<double> >
getFXSwapRates (double dSpotFX, double dPeriod, double dInitialTime);

/**
 * Prints data curve in the specified interval.
 *
 * @param rData The data curve.
 * @param dStartTime The initial time.
 * @param dInterval The length of the interval.
 * @param iPoints The number of points in the output.
 */
void print (const cfl::Function &rData, double dStartTime, double dInterval,
            unsigned iPoints = 10);

/**
 * Prints data curve in the specified interval.
 *
 * @param rData The data curve.
 * @param dStartTime The initial time.
 * @param dInterval The length of the interval.
 * @param iPoints The number of points in the output.
 */
void print (const std::function<double (double)> &rData, double dStartTime,
            double dInterval, unsigned iPoints = 10);

/**
 * Prints data curve in the specified interval.
 *
 * @param rTitle The title of the table.
 * @param rData The data curve.
 * @param dStartTime The initial time.
 * @param dInterval The length of the interval.
 * @param iPoints The number of points in the output.
 */
void print (const std::string &rTitle, const cfl::Function &rData,
            double dStartTime, double dInterval, unsigned iPoints = 10);

/**
 * Prints data curve in the specified interval.
 *
 * @param rTitle The title of the table.
 * @param rData The data curve.
 * @param rErr The error curve.
 * @param dStartTime The initial time.
 * @param dInterval The length of the interval.
 * @param iPoints The number of points in the output.
 */
void print (const std::string &rTitle, const cfl::Function &rData,
            const cfl::Function &rErr, double dStartTime, double dInterval,
            unsigned iPoints = 10);

/**
 * Prints table with input data.
 *
 * @param sTitle The title of the table.
 * @param rTimes The vector of arguments (usually, times).
 * @param rValues The vector of values.
 * @param sTimes The name of argument.
 * @param sValues The name of values.
 */
void print (const std::string &sTitle, const std::vector<double> &rTimes,
            const std::vector<double> &rValues,
            const std::string &sTimes = "time",
            const std::string &sValues = "value");

/**
 * Prints the result of a least-squares fit.
 *
 * @param rParam The result of least-squares fit.
 */
void printFit (const cfl::FitParam &rParam);

/** @} */
} // namespace Data
} // namespace test

#endif // of __test_all_Data_hpp__
