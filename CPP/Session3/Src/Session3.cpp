//
// Created by Jelmer Bennema on 2/28/24.
//

#include "Session3/Session3.hpp"
#include <iostream> // for std::cout
#include <numeric>


cfl::Function
prb::discountApproxFit(const std::vector<double> &rDiscountTimes, const std::vector<double> &rDiscountFactors,
                       double dInitialTime, cfl::Fit &rFit, cfl::Function &rErr) {

    // check preconditions hold
            PRECONDITION(std::is_sorted(rDiscountTimes.begin(), rDiscountTimes.end()));
            PRECONDITION(rDiscountTimes.size() == rDiscountFactors.size());
            PRECONDITION(dInitialTime <= rDiscountTimes.front());

    // construct weights vector
    std::vector<double> weights(rDiscountTimes.size());
    std::transform(rDiscountTimes.begin(), rDiscountTimes.end(), rDiscountFactors.begin(), weights.begin(),
                   [dInitialTime](double t, double d) {
                       // deal with 0/0 singularity at t = dInitialTime
                       return d * d * (t - dInitialTime) * (t - dInitialTime);
                   });

    // construct market yields
    std::vector<double> yields(rDiscountTimes.size());
    std::transform(rDiscountTimes.begin(), rDiscountTimes.end(), rDiscountFactors.begin(), yields.begin(),
                   [dInitialTime](double t, double d) {
                       // deal with singularity at t = dInitialTime
                       if (t == dInitialTime) {
                           return -std::log(d + cfl::EPS) / cfl::EPS;
                       }
                       return -std::log(d) / (t - dInitialTime);
                   });

    // perform fit
    rFit.assign(rDiscountTimes.begin(), rDiscountTimes.end(),
                yields.begin(), weights.begin(), true);

    auto fit = rFit.fit(); // fit the yields
    cfl::Function uDF = cfl::Data::discount(fit, dInitialTime);
    cfl::Function uT([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);
    rErr = rFit.err() * uDF * uT; // error for fitted discount curve

    return uDF; // return fitted discount curve
}


cfl::Function
prb::forwardCarryFit(double dSpot, const std::vector<double> &rDeliveryTimes, const std::vector<double> &rForwardPrices,
                     double dInitialTime, cfl::Fit &rFit, cfl::Function &rErr) {

            PRECONDITION(rDeliveryTimes.size() == rForwardPrices.size());
            PRECONDITION(dInitialTime <= rDeliveryTimes.front());
            PRECONDITION(std::is_sorted(rDeliveryTimes.begin(), rDeliveryTimes.end()));

    // construct cost of carry rates vector
    std::vector<double> q(rDeliveryTimes.size());
    std::transform(rDeliveryTimes.begin(), rDeliveryTimes.end(), rForwardPrices.begin(), q.begin(),
                   [dInitialTime, dSpot](double t, double F) {
                       return std::log(F / dSpot) / (t - dInitialTime);
                   });

    // fit cost of carry rates
    rFit.assign(rDeliveryTimes.begin(), rDeliveryTimes.end(), q.begin());
    auto fit = rFit.fit();

    cfl::Function forward_Curve = cfl::Data::forward(dSpot, fit, dInitialTime);
    cfl::Function delta_T = cfl::Function([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);

    rErr = forward_Curve * rFit.err() * delta_T; // error for fitted forward curve


    return forward_Curve; // return fitted forward curve
}

// Shape function to deal with singularities 0/0
std::function<double(double)> shape1 = [](double dX) {
    if (dX == 0) {
        return 1 - dX / 2 + dX * dX / 6;
    } else {
        return (1 - std::exp(-dX)) / dX;
    }
};

cfl::Function basis(double dLambda, double dInitialTime) {
    std::function<double(double)> uB = [dLambda, dInitialTime](double dT) {
        double dX = dLambda * (dT - dInitialTime);
        return shape1(dX);
    };
    return cfl::Function(uB, dInitialTime);
}


cfl::Function
prb::forwardBlackFit(double dSpot, const std::vector<double> &rTimes, const std::vector<double> &rForwards,
                     double dLambda, double dSigma, double dInitialTime, cfl::Function &rErr, cfl::FitParam &rParam) {

            PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end()));
            PRECONDITION(dInitialTime <= rTimes.front());
            PRECONDITION((dSigma > 0) && (dLambda >= 0));

    // fit to find theta
    cfl::Function uBB = basis(dLambda, dInitialTime);
    cfl::Function uF = basis(2 * dLambda, dInitialTime) * 0.5 * dSigma * dSigma;

    cfl::Fit uFit = cfl::NFit::linear(uBB, uF); // create fit object for linear fit

    cfl::Function uForward = forwardCarryFit(dSpot, rTimes, rForwards, dInitialTime, uFit, rErr);
    rParam = uFit.param(); // get fitted coefficients and their covariance matrix

    return uForward;
}

cfl::Function
prb::forwardBlackMultiFit(double dSpot, const std::vector<double> &rTimes, const std::vector<double> &rForwards,
                          const std::vector<double> &rLambda, const std::vector<double> &rSigma, double dInitialTime,
                          cfl::Function &rErr, cfl::FitParam &rParam) {

            PRECONDITION(std::is_sorted(rLambda.begin(), rLambda.end()));
            PRECONDITION(dInitialTime < rTimes.front());
            PRECONDITION(rLambda.size() == rSigma.size());
            PRECONDITION(
            (std::all_of(rSigma.begin(), rSigma.end(), [](double d) { return d > 0; }))); // confirm sigmas are positive
            PRECONDITION((std::all_of(rLambda.begin(), rLambda.end(),
                                      [](double d) { return d >= 0; }))); // confirm lambdas are non-negative

// construct basis functions and free function

    std::vector<cfl::Function> uBase;
    for (unsigned i = 0; i < rLambda.size(); ++i) {
        uBase.push_back(basis(rLambda[i], dInitialTime));
    }

    cfl::Function uG(0., dInitialTime);
    uG = std::inner_product(rSigma.begin(), rSigma.end(), rLambda.begin(),
                            uG, std::plus<cfl::Function>(),
                            [dInitialTime](double dSigma, double dLambda) {
                                return (0.5 * dSigma * dSigma)
                                       * basis(2. * dLambda, dInitialTime);
                            });
    // fit to find theta
    cfl::Fit uFit = cfl::NFit::linear(uBase, uG); // create fit object for linear fit

    cfl::Function uForward = forwardCarryFit(dSpot, rTimes, rForwards, dInitialTime, uFit, rErr);
    rParam = uFit.param(); // get fitted coefficients and their covariance matrix

    return uForward;


}