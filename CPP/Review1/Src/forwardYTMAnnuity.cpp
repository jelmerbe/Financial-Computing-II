//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

cfl::Function prb::forwardYTMAnnuity(double dRate, double dPeriod, double dMaturity, const cfl::Function &rDiscount,
                                     double dInitialTime, const cfl::RootD &rRoot) {

    // check preconditions
    PRECONDITION(dMaturity > dInitialTime);

    std::function<double(double)> yields
    = [dRate, dPeriod, dMaturity, &rDiscount, rRoot, dInitialTime](double dT) {

        std::function<double(double)> F
                = [dRate, dPeriod, dMaturity, &rDiscount, dInitialTime, dT](double dYield) {
                    double annuity = 0.0, time = dMaturity;
                    while (time > dT) {
                        annuity += dRate * dPeriod * exp(-dYield * (time - dT));
                        time -= dPeriod;
                    }
                    double target= forwardAnnuity(dRate, dPeriod, dMaturity, rDiscount, dInitialTime, false)(dT);
                    return annuity - target; // dirty price
                };

        std::function<double(double)> DF
                = [dRate, dPeriod, dMaturity, dInitialTime, dT](double dYield) {
                    double deriv = 0.0, time = dMaturity;
                    while (time > dT) {
                        deriv += dRate * dPeriod * exp(-dYield * (time - dT)) * -(time - dT);
                        time -= dPeriod;
                    }
                    return deriv; // derivative of dirty price
                };


        cfl::Function rF(F);
        cfl::Function rDF(DF);
        return rRoot.find(rF, rDF, dRate);
    };


    return cfl::Function(yields, dInitialTime, dMaturity);
}