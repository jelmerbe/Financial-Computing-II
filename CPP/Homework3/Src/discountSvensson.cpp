//
// Created by Jelmer Bennema on 3/1/24.
//

#include "Homework3/Homework3.hpp"

// Shape functions to deal with singularities 0/0
std::function<double(double)> shape1 = [](double dX) {
    if (dX == 0) {
        return 1 - dX / 2 + dX * dX / 6;
    }
    return (1 - std::exp(-dX)) / dX;
};

double shape2(double dX) {
    if (dX > cfl::EPS) {
        return (1 - exp(-dX)) / dX - exp(-dX);
    }
    return dX / 2 - dX * dX / 3;
}

// Basis functions
cfl::Function basis1(double dLambda1, double dInitialTime) {
    std::function<double(double)> B = [dLambda1, dInitialTime](double dT) {
        double dX = dLambda1 * (dT - dInitialTime);
        return shape1(dX);
    };
    return cfl::Function(B, dInitialTime);
}

cfl::Function basis2(double dLambda, double dInitialTime) {
    std::function<double(double)> B = [dLambda, dInitialTime](double dT) {
        double dX = dLambda * (dT - dInitialTime);
        return shape2(dX);
    };
    return cfl::Function(B, dInitialTime);
}


cfl::Function
prb::discountSvenssonFit(const std::vector<double> &rTimes, const std::vector<double> &rDF, double dLambda1,
                         double dLambda2, double dInitialTime, cfl::Function &rErr, cfl::FitParam &rParam) {

    // check preconditions for input
            PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end())); // confirm times are ordered
            PRECONDITION(dInitialTime < rTimes.front()); // t_0 < t_1
            PRECONDITION((dLambda1 > 0) && (dLambda2 > 0) && (dLambda2 != dLambda1)); // check lambda values

    // construct market yields
    std::vector<double> uYields(rTimes.size());
    std::transform(rTimes.begin(), rTimes.end(), rDF.begin(), uYields.begin(), [dInitialTime](double dT, double dD) {
        if (dT == dInitialTime) {
            return -std::log(dD + cfl::EPS) / cfl::EPS;
        } else {
            return -std::log(dD) / (dT - dInitialTime);
        }
    });

    // create vector of basis functions
    std::vector<cfl::Function> uBasis(4);
    uBasis[0] = 1.0;
    uBasis[1] = basis1(dLambda1, dInitialTime);
    uBasis[2] = basis2(dLambda1, dInitialTime);
    uBasis[3] = basis2(dLambda2, dInitialTime);

    // construct linear fit object
    cfl::Fit uFit = cfl::NFit::linear(uBasis);

    uFit.assign(rTimes.begin(), rTimes.end(), uYields.begin());
    auto fit = uFit.fit();

    cfl::Function uD = cfl::Data::discount(uFit.fit(), dInitialTime);
    cfl::Function uT = cfl::Function([dInitialTime](double dT) { return dT - dInitialTime; }, dInitialTime);
    rErr = uD * uFit.err() * uT;
    rParam = uFit.param();

    return cfl::Data::discount(uFit.fit(), dInitialTime);
}
