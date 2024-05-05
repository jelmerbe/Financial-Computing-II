
#include "Exam/Exam.hpp"

cfl::Function
prb::discountSwapLogLinInterp(const std::vector<double> &rSwapRates, double dPeriod, double dInitialTime) {

    // check preconditions
            PRECONDITION(dPeriod > 0);
            PRECONDITION(dInitialTime >= 0);
            PRECONDITION(rSwapRates.size() > 0);

    // create time vector
    std::vector<double> times(rSwapRates.size() + 1);
    times.front() = dInitialTime;
    std::transform(times.begin(), times.end() - 1, times.begin() + 1,
                   [dPeriod](double t) { return t + dPeriod; });

    // find market discount rates for given swap rates
    double sum = 0.0;
    std::vector<double> discounts(rSwapRates.size() + 1);
    discounts.front() = 1;
    for (size_t i = 1; i < rSwapRates.size() +1 ; ++i) {
        discounts[i] = (1 - rSwapRates[i-1] * dPeriod * sum) / (1 + rSwapRates[i-1] * dPeriod);
        sum += discounts[i]; // sum of discounts
    }

    // log all discount rates (since we are performing linear interpolation on logs)
    std::transform(discounts.begin(), discounts.end(), discounts.begin(),
                   [](double d) { return log(d);});

    // perform interpolation (on logs)
    cfl::Interp rInterp = cfl::NInterp::linear(); // linear interpolation
    rInterp.assign(times.begin(), times.end(), discounts.begin());

    return exp(rInterp.interp());
}

