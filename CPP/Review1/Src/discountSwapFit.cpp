//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

cfl::Function
prb::discountSwapFit(const std::vector<double> &rSwapRates, double dPeriod, double dInitialTime, cfl::Fit &rFit,
                     cfl::Function &rErr) {

    // find market discount rates for given swap rates
    double sum = 0.0;
    std::vector<double> discounts(rSwapRates.size());
    for (size_t i = 0; i < rSwapRates.size(); ++i) {
        discounts[i] = (1 - rSwapRates[i] * dPeriod * sum) / (1 + rSwapRates[i] * dPeriod);
        sum += discounts[i]; // sum of discounts
    }

    // construct time vector
    std::vector<double> times;
    times.reserve(rSwapRates.size());
    for (int i = 0; i < rSwapRates.size(); ++i) {
        times.push_back(dInitialTime + (i + 1) * dPeriod);
    }

    // find market yields
    std::vector<double> yields(rSwapRates.size());
    std::transform(discounts.begin(), discounts.end(), times.begin(), yields.begin(),
                   [dInitialTime](double discount, double time) { return -log(discount) / (time - dInitialTime); });

    // fit market yields
    rFit.assign(times.begin(), times.end(), yields.begin());
    auto uYieldCurve = rFit.fit(); // fit the yields

    // find error and params
    auto discountCurve = cfl::Data::discount(uYieldCurve, dInitialTime);
    cfl::Function uT([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);
    rErr = discountCurve * rFit.err() * uT;

    return discountCurve;
}