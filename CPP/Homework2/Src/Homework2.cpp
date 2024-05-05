//
// Created by Jelmer Bennema on 2/27/24.
//

#include "Homework2/Homework2.hpp"

double
prb::yieldTMCashFlow(const std::vector<double> &rPayments, const std::vector<double> &rPaymentTimes, double dValue,
                     double dInitialTime, double dY0, double dY1, const cfl::Root &rRoot) {

            PRECONDITION(std::is_sorted(rPaymentTimes.begin(), rPaymentTimes.end()));
            PRECONDITION(dY0 < dY1);


    // construct function f (bracketing method)
    std::function<double(double)> rF
            = [rPayments, rPaymentTimes, dValue, dInitialTime](double yield) {
                auto time = std::lower_bound(rPaymentTimes.begin(), rPaymentTimes.end(), dInitialTime);
                auto value = rPayments.begin() + std::distance(rPaymentTimes.begin(), time);

                double presentValue = 0.0;

                for (; time < rPaymentTimes.end(); value++, time++) {
                    presentValue += *value * exp(-yield * (*time - dInitialTime));
                }
                return presentValue - dValue;
            };

    cfl::Function F(rF, dY0, dY1);

    // find roots using given function
    return rRoot.find(F, dY0, dY1);
}

cfl::Function prb::forwardYTMCashFlow(const std::vector<double> &rPayments, const std::vector<double> &rPaymentTimes,
                                      const cfl::Function &rForward, double dInitialTime, double dY0, double dY1,
                                      const cfl::Root &rRoot) {


    std::function<double(double)> FT
            = [rPayments, rPaymentTimes, rForward, dInitialTime, dY0, dY1, rRoot](double dT) {

                double Value = rForward(dT);
                return prb::yieldTMCashFlow(rPayments, rPaymentTimes, Value, dT, dY0, dY1, rRoot);
            };

    return cfl::Function(FT, dInitialTime);

}

cfl::Function
prb::volatilityVarInterp(const std::vector<double> &rTimes, const std::vector<double> &rVols, double dInitialTime,
                         cfl::Interp &rInterp) {

            PRECONDITION(std::is_sorted(rTimes.begin(), rTimes.end())); // confirm times are ordered
            PRECONDITION(dInitialTime < rTimes.front()); // t_0 < t_1

    // find times vector
    std::vector<double> times(rTimes.size() + 1);
    times.front() = dInitialTime;
    std::copy(rTimes.begin(), rTimes.end(), times.begin() + 1);

    // find v(t), the interpolation of market variances
    std::vector<double> v(rVols.size() + 1);
    v.front() = 0.0;
    std::transform(rVols.begin(), rVols.end(), rTimes.begin(), v.begin() + 1,
                   [dInitialTime](double vol, double time) {
                       return vol * vol * (time - dInitialTime);
                   });

    // perform interpolation
    rInterp.assign(times.begin(), times.end(), v.begin());
    cfl::Function v_interpolator = rInterp.interp();

    // write function to return the volatility curve
    std::function<double(double)> vol
            = [dInitialTime, v_interpolator](double dT) {
                // deal with singularity of 0/0 at t = t_0
                if (dT == dInitialTime) {
                    return std::sqrt(v_interpolator(dT + cfl::EPS) / (dT - dInitialTime + cfl::EPS));
                } else {
                    return std::sqrt(v_interpolator(dT) / (dT - dInitialTime));
                }
            };

    return cfl::Function(vol, dInitialTime);

}

cfl::Function prb::volatilityVarSteffenInterp(const std::vector<double> &rTimes, const std::vector<double> &rVols,
                                              double dInitialTime) {

    cfl::Interp steffen = cfl::NInterp::steffen();
    return prb::volatilityVarInterp(rTimes, rVols, dInitialTime, steffen);

}
