//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

cfl::Function prb::forwardFXCarryLinInterp(double dSpotFX, const std::vector<double> &rDiscountTimes,
                                           const std::vector<double> &rDomesticDiscountFactors,
                                           const std::vector<double> &rForeignDiscountFactors, double dInitialTime) {

    // find market cost of carry rates
    std::vector<double> rCostOfCarry(rDiscountTimes.size());
    // find log (foreign/domestic) discount factors
    std::transform(rDomesticDiscountFactors.begin(), rDomesticDiscountFactors.end(),
                   rForeignDiscountFactors.begin(), rCostOfCarry.begin(),
                   [](double dDomestic, double dForeign) {
                       return std::log(dForeign / dDomestic);
                   });
    // divide by time difference to get final cost of carry rates
    std::transform(rCostOfCarry.begin(), rCostOfCarry.end(),
                   rDiscountTimes.begin(), rCostOfCarry.begin(),
                   [dInitialTime](double dCost, double dT) {
                       return dCost / (dT - dInitialTime);
                   });

    // perform linear interpolation to get cost of carry curve
    cfl::Interp rInterp = cfl::NInterp::linear(); // linear interpolation
    rInterp.assign(rDiscountTimes.begin(), rDiscountTimes.end(), rCostOfCarry.begin());
    auto vInterpolator = rInterp.interp();

    // find forward exchange rate curve
    std::function<double(double)> F
            = [dSpotFX, rInterp, dInitialTime, vInterpolator, rDiscountTimes](double dT) {

        double CoC; // cost of carry
                if (dT < rDiscountTimes.front()) {
                    CoC = vInterpolator(rDiscountTimes.front()); // cost of carry is constant in [t_0, t_1]
                }
                else {
                    CoC = vInterpolator(dT);
                }

                return dSpotFX * std::exp(CoC * (dT - dInitialTime));
            };

    return cfl::Function(F, dInitialTime);
}