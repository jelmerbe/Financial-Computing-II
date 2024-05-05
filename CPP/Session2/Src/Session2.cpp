//
// Created by Jelmer Bennema on 2/26/24.
//

#include "Session2/Session2.hpp"
#include <vector>

double prb::couponBond(double dRate, double dPeriod, double dMaturity, double dYTM, double dInitialTime, bool bClean) {

            PRECONDITION(dMaturity >= dInitialTime);
            PRECONDITION(dPeriod >= 0);

    double dT = dMaturity - dInitialTime;
    double dDF = exp(-dYTM * dT);
    double dC = dRate * dPeriod;
    double dP = (1. + dC) * dDF;
    double dG = exp(dYTM * dPeriod);
    dT -= dPeriod;
    while (dT > 0) {
        dDF *= dG;
        dP += dC * dDF;
        dT -= dPeriod;
    }
            ASSERT (dT <= 0);
    if (bClean) {
        dP += dRate * dT;
    }
    return dP;
}

double prb::durationCouponBond(double dRate, double dPeriod, double dMaturity, double dYTM, double dInitialTime) {
            PRECONDITION(dMaturity >= dInitialTime);
            PRECONDITION((dRate >= 0) && (dPeriod > 0));

    double dT = dMaturity - dInitialTime;
    double dDF = exp(-dYTM * dT);
    double dC = dRate * dPeriod;
    double dDur = (1. + dC) * dT * dDF;
    double dG = exp(dYTM * dPeriod);
    dT -= dPeriod;
    while (dT > 0) {
        dDF *= dG;
        dDur += dT * dC * dDF;
        dT -= dPeriod;
    }
            ASSERT (dT <= 0);
    return dDur;
}

double prb::yieldTMCouponBond(double dRate, double dPeriod, double dMaturity, double dInitialTime, double dPrice,
                              double dYield0, double dErr) {

    unsigned int i = 1;
    double Y1 = 1e6, Y0 = dYield0;
    double F, dF;

    // main Newton loop
    while (std::abs(Y1 - Y0) > dErr) {
        F = dPrice - couponBond(dRate, dPeriod, dMaturity, Y0, dInitialTime, false);
        dF = durationCouponBond(dRate, dPeriod, dMaturity, Y0, dInitialTime);
        Y1 = Y0 - F / dF;
        Y0 = Y1;
    }

    return Y1;
}

double prb::yieldTMCouponBond(double dRate, double dPeriod, double dMaturity, double dInitialTime, double dPrice,
                              double dYield0, const cfl::RootD &rRootD) {


    std::function<double(double)> F
            = [dRate, dPeriod, dMaturity, dInitialTime, dPrice](double yield) {
                return dPrice - couponBond(dRate, dPeriod, dMaturity, yield, dInitialTime, false);
            };

    std::function<double(double)> DF
            = [dRate, dPeriod, dMaturity, dInitialTime](double yield) {
                return durationCouponBond(dRate, dPeriod, dMaturity, yield, dInitialTime);
            };


    cfl::Function rF(F, dYield0);
    cfl::Function rDF(DF, dYield0);

    return rRootD.find(rF, rDF, dYield0);
}

std::function<double(double, double)>
yield(double dInitialTime) {
    return [dInitialTime](double dT, double dD) {
                PRECONDITION (dT > dInitialTime + cfl::EPS);
        return -std::log(dD) / (dT - dInitialTime);
    };
}

cfl::Function prb::discountYieldInterp(const std::vector<double> &rTimes, const std::vector<double> &rDF, double dR0,
                                       double dInitialTime, cfl::Interp &rInterp) {

            PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end())); // confirm times are ordered
            PRECONDITION(dInitialTime < rTimes.front()); // t_0 < t_1

    // construct times vector
    std::vector<double> timeS(rTimes.size() + 1);
    timeS.front() = dInitialTime;
    std::copy(rTimes.begin(), rTimes.end(), timeS.begin() + 1);

    // construct yields
    std::vector<double> uYields(rTimes.size());
    uYields.front() = dR0;
    std::transform(rTimes.begin(), rTimes.end(), rDF.begin(),
                   uYields.begin() + 1, yield(dInitialTime));

    // perform interpolation
    rInterp.assign(timeS.begin(), timeS.end(), uYields.begin());
    cfl::Function yield_interpolator = rInterp.interp();

    return cfl::Data::discount(yield_interpolator, dInitialTime);

}

cfl::Function
prb::discountYieldSteffenInterp(const std::vector<double> &rTimes, const std::vector<double> &rDF, double dR0,
                                double dInitialTime) {

    cfl::Interp steffen = cfl::NInterp::steffen();
    return prb::discountYieldInterp(rTimes, rDF, dR0, dInitialTime, steffen);
};
