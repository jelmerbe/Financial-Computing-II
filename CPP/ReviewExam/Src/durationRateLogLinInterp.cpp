//
// Created by Jelmer Bennema on 3/6/24.
//

#include "ReviewExam/ReviewExam.hpp"

cfl::Function prb::discountRateLogLinInterp(const std::vector<double> &rRatePeriods, const std::vector<double> &rRates,
                                            double dInitialTime) {

    // create vector of times
    std::vector<double> times(rRatePeriods.size() + 1);
    times.front() = dInitialTime;
    std::transform(rRatePeriods.begin(), rRatePeriods.end(), times.begin() + 1,
                   [dInitialTime](double period) { return period + dInitialTime; });

    // find market discounts
    std::vector<double> discounts(rRatePeriods.size() + 1);
    discounts.front() = log(1);
    std::transform(rRates.begin(), rRates.end(), rRatePeriods.begin(), discounts.begin() + 1,
                   [](double rate, double period) { return log(1 / (1 + rate * period)); });

    // apply interpolation
    cfl::Interp rInterp = cfl::NInterp::linear(); // linear interpolation
    rInterp.assign(times.begin(), times.end(), discounts.begin());

    cfl::Function v_interpolator = rInterp.interp();

    return exp(v_interpolator);
}