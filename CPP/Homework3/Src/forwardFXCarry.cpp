//
// Created by Jelmer Bennema on 3/1/24.
//

#include "Homework3/Homework3.hpp"

cfl::Function prb::forwardFXCarryFit(double dSpotFX, const std::vector<double> &rTimes,
                                     const std::vector<double> &rDomesticDiscountFactors,
                                     const std::vector<double> &rForeignDiscountFactors, double dInitialTime,
                                     cfl::Fit &rFit, cfl::Function &rErr) {

    // check preconditions hold
            PRECONDITION(rTimes.size() == rDomesticDiscountFactors.size());
            PRECONDITION(rTimes.size() == rForeignDiscountFactors.size());
            PRECONDITION(dInitialTime <= rTimes.front());
            PRECONDITION(dSpotFX > 0);

    // find domestic and foreign interest rates
    std::vector<double> rDomesticInterest(rTimes.size()), rForeignInterest(rTimes.size());
    std::transform(rDomesticDiscountFactors.begin(), rDomesticDiscountFactors.end(),
                   rTimes.begin(), rDomesticInterest.begin(),
                   [dInitialTime](double dDF, double dT) {
                       return -std::log(dDF) / (dT - dInitialTime);
                   });
    std::transform(rForeignDiscountFactors.begin(), rForeignDiscountFactors.end(),
                   rTimes.begin(), rForeignInterest.begin(),
                   [dInitialTime](double dDF, double dT) {
                       return -std::log(dDF) / (dT - dInitialTime);
                   });

    // construct cost of carry rates vector
    std::vector<double> q(rTimes.size());
    std::transform(rDomesticInterest.begin(), rDomesticInterest.end(),
                   rForeignInterest.begin(), q.begin(),
                   [](double dDomestic, double dForeign) {
                       return dDomestic - dForeign;
                   });

    // fit to find cost of carry curve
    rFit.assign(rTimes.begin(), rTimes.end(), q.begin());
    auto fit = rFit.fit(); // fit the cost of carry rates

    // construct forward exchange rate curve
    std::function<double(double)> F
            = [dSpotFX, fit, dInitialTime](double dT) {
                return dSpotFX * std::exp(fit(dT) * (dT - dInitialTime));
            };
    cfl::Function uT([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);
    cfl::Function FX (F, dInitialTime);

    rErr = uT * rFit.err() * FX; // error for fitted cost of carry curve

    return FX; // return fitted forward fx curve
}