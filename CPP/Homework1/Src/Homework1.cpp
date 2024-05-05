//
// Created by Jelmer Bennema on 2/23/24.
//

#include "Homework1/Homework1.hpp"
#include "cfl/Data.hpp"
#include <vector>
#include <iostream>

/* write approximations in case of 0/0 singularity.
 it takes in 2 * dLambda * (T - t) and checks for values */
double shape1(double dX) {
    if (dX > cfl::TIME_EPS) {
        return (1 - exp(-dX)) / dX;
    } else {
        return 1 - dX / 2 + dX * dX / 6;
    }
}

cfl::Function prb::volatilityVar(const cfl::Function &rVar, double dInitialTime) {

            PRECONDITION(dInitialTime >= 0);
    // setup lambda function for dInitialTime
    std::function<double(double)> vol
            = [rVar, dInitialTime](double dT) {
                        ASSERT(dT >= dInitialTime);
                if (dT - dInitialTime < cfl::EPS) {
                    return sqrt(rVar(dT + cfl::EPS) / (dT - dInitialTime + cfl::EPS));
                } else {
                    return sqrt(rVar(dT) / (dT - dInitialTime));
                }

            };
    // return function for all times (using cfl library)
    return cfl::Function(vol, dInitialTime);
}


cfl::Function prb::carryBlack(double dTheta, double dLambda, double dSigma, double dInitialTime) {
            PRECONDITION((dLambda >= 0) && (dSigma >= 0));

    std::function<double(double)> carry
            = [dTheta, dLambda, dSigma, dInitialTime](double dT) {
                        ASSERT(dT >= dInitialTime);
                double dX1 = dLambda * (dT - dInitialTime);
                double dX2 = 2 * dLambda * (dT - dInitialTime);

                return dTheta * shape1(dX1) + dSigma * dSigma / 2 * shape1(dX2);
            };

    return cfl::Function(carry, dInitialTime);
}

double prb::swapRate(double dPeriod, unsigned int iPayments, const cfl::Function &rDiscount, double dInitialTime) {
            PRECONDITION(iPayments >= 0);
            PRECONDITION(dPeriod > 0);

    double PV_Float, accrual_Factor = 0.0;
    double time = dInitialTime;

    PV_Float = rDiscount(dInitialTime) - rDiscount(dInitialTime + dPeriod * iPayments);

    for (unsigned int m = 0; m < iPayments; m++) {
        time += dPeriod * m;
        accrual_Factor += dPeriod * rDiscount(time);
    }

    return PV_Float / accrual_Factor;


}

cfl::Function prb::forwardCashFlow(const std::vector<double> &rPayments, const std::vector<double> &rPaymentTimes,
                                   const cfl::Function &rDiscount, double dInitialTime) {

            PRECONDITION(rPaymentTimes.front() > dInitialTime);
            PRECONDITION(dInitialTime >= 0);
            PRECONDITION(rPaymentTimes.size() == rPayments.size());

    // write F for specific dT
    std::function<double(double)> forward
            = [rPayments, rPaymentTimes, rDiscount, dInitialTime](double dT) {
                        ASSERT(dT >= dInitialTime);

                double PV = 0.0;

                auto time = std::lower_bound(rPaymentTimes.begin(), rPaymentTimes.end(), dT);
                for (int i = 0; i < rPaymentTimes.size(); i++) {
                    if (rPaymentTimes[i] > dT) {
                        PV += rPayments[i] * rDiscount(rPaymentTimes[i]);
                    }
                }

                return PV / rDiscount(dT);
            };

    return cfl::Function(forward, dInitialTime);

}