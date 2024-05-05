//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"


// shape function which deals with singularities of 0/0
double shape1 (double dX) {
    if (dX == 0) {
        return 1 - dX / 2 + dX*dX / 6; // quadratic approximation
    } else {
        return (1 - exp(-dX)) / dX;
    }
}

cfl::Function prb::yieldVasicek(double dTheta, double dLambda, double dSigma, double dR0, double dInitialTime) {

    // check preconditions
    PRECONDITION(dLambda > 0);
    PRECONDITION(dSigma > 0);
    PRECONDITION(dInitialTime >= 0);

    // find yield function
    std::function<double(double)> fYield = [dTheta, dLambda, dSigma, dR0, dInitialTime](double dT) {
        double dX = dLambda * (dT - dInitialTime);
        double A = shape1(dX); // A(t)
        double B = shape1(2 * dX); // B(t)

        return dR0 * A + dTheta / dLambda * (1 - A) - dSigma * dSigma / (2 * dLambda * dLambda) * (1 - 2 * A + B);
    };

    return cfl::Function(fYield, dInitialTime);
}