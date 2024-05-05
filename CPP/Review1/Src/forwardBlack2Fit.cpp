//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

// Shape function to deal with singularities 0/0
std::function<double(double)> shape = [](double dX) {
    if (dX == 0) {
        return 1 - dX / 2 + dX * dX / 6;
    } else {
        return (1 - std::exp(-dX)) / dX;
    }
};

cfl::Function
prb::forwardBlack2Fit(double dSpot, const std::vector<double> &rTimes, const std::vector<double> &rForwards,
                      double dLambda1, double dSigma0, double dSigma1, double dInitialTime, cfl::Function &rErr,
                      cfl::FitParam &rParam) {

    // check preconditions
            PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end()));
            PRECONDITION((dLambda1 > 0) && (dSigma0 > 0) && (dSigma1 > 0));
            PRECONDITION(dSigma0 != dSigma1);
            PRECONDITION(dInitialTime < rTimes.front());

    // basis functions
    std::function<double(double)> rBasis1 = [](double dT) { return 1.0; };
    std::function<double(double)> rBasis2 = [dLambda1, dInitialTime](double dT) {
        double dX = dLambda1 * (dT - dInitialTime);
        return shape(dX);
    };


    // free function (constant)
    std::function<double(double)> rFree = [dSigma0, dSigma1, dLambda1, dInitialTime](double dT) {
        double dX = 2 * dLambda1 * (dT - dInitialTime);
        return dSigma0 * dSigma0 / 2 + dSigma1 * dSigma1 / 2 * shape(dX);
    };

    // find market cost of carry rates (vector)
    std::vector<double> rCostofCarry(rTimes.size());
    std::transform(rForwards.begin(), rForwards.end(), rTimes.begin(), rCostofCarry.begin(),
                   [dSpot, dInitialTime](double dForward, double dT) {
                       return std::log(dForward / dSpot) / (dT - dInitialTime);
                   });

    // construct skeleton of fit
    std::vector<cfl::Function> rBasisFunctions = {cfl::Function(rBasis1, dInitialTime),
                                                  cfl::Function(rBasis2, dInitialTime)};
    auto uFree = cfl::Function(rFree, dInitialTime);
    cfl::Fit rFit = cfl::NFit::linear(rBasisFunctions, uFree);

    // assign data to fit
    rFit.assign(rTimes.begin(), rTimes.end(), rCostofCarry.begin());
    auto CostCarryCurve = rFit.fit(); // fit the yields

    // find forward curve, error and params
    std::function<double(double)> vForward = [dSpot, CostCarryCurve, dInitialTime](double dT) {
        return dSpot * exp(CostCarryCurve(dT) * (dT - dInitialTime));
    };
    cfl::Function uT([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);
    cfl::Function ForwardCurve(vForward, dInitialTime);

    rErr = ForwardCurve * rFit.err() * uT;
    rParam = rFit.param();

    return cfl::Function(vForward, dInitialTime);
}