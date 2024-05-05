//
// Created by Jelmer Bennema on 3/6/24.
//

#include "ReviewExam/ReviewExam.hpp"

double prb::durationAnnuity(double dRate, double dPeriod, double dMaturity, double dYTM, double dInitialTime) {
    double dT = dMaturity;
    double dDur = 0.0;

    while (dT >  dInitialTime) {
        dDur += exp(-dYTM * (dT - dInitialTime)) * dRate * dPeriod * (dT - dInitialTime);
        dT -= dPeriod;
    }

    return dDur;
}

