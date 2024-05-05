//
// Created by Jelmer Bennema on 3/5/24.
//

#include   "Review1/Review1.hpp"

double
prb::yieldTMAnnuity(double dRate, double dPeriod, double dMaturity, double dInitialTime, double dPrice, double dYield0,
                    const cfl::RootD &rRootD) {

    // check preconditions
            PRECONDITION(dMaturity > dInitialTime);

    std::function<double(double)> F =
            [dRate, dPeriod, dMaturity, dPrice, dInitialTime](double dYield) {
                double annuity = 0.0, time = dMaturity;

                while (time > dInitialTime) {
                    annuity += exp(-dYield * (time - dInitialTime)) * dRate * dPeriod;
                    time -= dPeriod;
                }
                return annuity - dPrice; // dirty price
            };

    std::function<double(double)> DF =
            [dRate, dPeriod, dMaturity, dPrice, dInitialTime](double dYield) {
                double annuity = 0.0, time = dMaturity;

                while (time > dInitialTime) {
                    annuity += exp(-dYield * (time - dInitialTime)) * dRate * dPeriod * -(time - dInitialTime);
                    time -= dPeriod;
                }
                return annuity; // derivative of dirty price
            };

    cfl::Function rF(F); // create function object
    cfl::Function rDF(DF); // create function object

    return rRootD.find(rF, rDF, dYield0); // find yield to maturity
}
