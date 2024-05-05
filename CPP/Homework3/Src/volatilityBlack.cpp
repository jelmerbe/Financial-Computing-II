//
// Created by Jelmer Bennema on 3/1/24.
//

#include "Homework3/Homework3.hpp"

// Shape function to deal with singularities 0/0
std::function<double(double)> shape = [](double dX) {
    if (dX == 0) {
        return 1 - dX / 2 + dX * dX / 6;
    } else {
        return (1 - std::exp(-dX)) / dX;
    }
};

cfl::Function
prb::volatilityBlackFit(const std::vector<double> &rTimes, const std::vector<double> &rVols, double dLambda,
                        double dInitialTime, cfl::Function &rErr, cfl::FitParam &rParam) {

    // check preconditions
    PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end())); // confirm times are ordered
    PRECONDITION(dInitialTime < rTimes.front()); // t_0 < t_1
    PRECONDITION(std::all_of(rVols.begin(), rVols.end(), [](double d) { return d > 0; })); // confirm vols are positive

    // fit to find theta
    std::function<double(double)> basis = [dLambda, dInitialTime](double dT) {
        double dX = 2 * dLambda * (dT - dInitialTime);
        return sqrt(shape(dX));
    };

    cfl::Function uBasis = cfl::Function(basis, dInitialTime);
    cfl::Fit uFit =  cfl::NFit::linear(uBasis);

    // perform fit
    uFit.assign(rTimes.begin(), rTimes.end(), rVols.begin());
    auto vol_curve = uFit.fit(); // fit the yields

    // error and params
    rErr = uFit.err();
    rParam = uFit.param();

    return vol_curve;
}