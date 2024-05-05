//
// Created by Jelmer Bennema on 3/1/24.
//

#include "Homework3/Homework3.hpp"

cfl::Function
prb::discountRateFit(const std::vector<double> &rPeriods, const std::vector<double> &rRates, double dInitialTime,
                     cfl::Fit &rFit, cfl::Function &rErr) {

            PRECONDITION(rPeriods.size() == rRates.size());
            PRECONDITION(std::is_sorted(rPeriods.begin(), rPeriods.end()));

    //construct time vector
    std::vector<double> times(rPeriods.size());
    std::transform(rPeriods.begin(), rPeriods.end(), times.begin(), [dInitialTime](double per) {
        return dInitialTime + per;
    });

    // construct discounts --> market yields
    std::vector<double> discounts(rPeriods.size()), yields(rPeriods.size());
    std::transform(rPeriods.begin(), rPeriods.end(), rRates.begin(),
                   discounts.begin(), [](double delta, double r) {
                return 1 / (1 + delta * r);
            });

    std::transform(rPeriods.begin(), rPeriods.end(), discounts.begin(), yields.begin(),
                   [](double delta, double d) {
                return std::pow((1 / d), 1 / delta) - 1;
            });

    // fit market yields to find yield curve
    rFit.assign(times.begin(), times.end(), yields.begin());
    auto fit = rFit.fit();

    cfl::Function uDF = cfl::Data::discount(fit, dInitialTime);
    cfl::Function uT([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);

    rErr = uDF * rFit.err() * uT; // error for fitted discount curve

    return uDF; // return fitted discount curve
}