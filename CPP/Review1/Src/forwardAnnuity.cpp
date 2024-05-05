//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

cfl::Function
prb::forwardAnnuity(double dRate, double dPeriod, double dMaturity, const cfl::Function &rDiscount, double dInitialTime,
                    bool bClean) {

    // check preconditions
            PRECONDITION(dRate >= 0);
            PRECONDITION(dPeriod > 0);
            PRECONDITION(dMaturity > dInitialTime);

    // compute forward annuity price for given time dT
    std::function<double(double)> fForwardAnnuity
            = [dRate, dPeriod, dMaturity, &rDiscount, dInitialTime, bClean](double dT) {
                double annuity = 0.0;
                double time = dMaturity;

                while (time > dT) {
                    annuity += rDiscount(time) / rDiscount(dT) * dRate * dPeriod;
                    time -= dPeriod;
                }

                if (bClean) {
                    return annuity - dRate * (dT - time); // clean price
                }

                return annuity; // dirty price
            };

    return cfl::Function(fForwardAnnuity, dInitialTime);
}
