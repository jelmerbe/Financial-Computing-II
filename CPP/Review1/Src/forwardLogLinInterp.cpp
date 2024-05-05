//
// Created by Jelmer Bennema on 3/5/24.
//

#include "Review1/Review1.hpp"

cfl::Function prb::forwardLogLinInterp(double dSpot, const std::vector<double> &rDeliveryTimes,
                                       const std::vector<double> &rForwardPrices, double dInitialTime) {

            PRECONDITION(rDeliveryTimes.size() == rForwardPrices.size());
            PRECONDITION(dInitialTime < rDeliveryTimes.front());

            // construct time vector (t0 - tM)
            std::vector<double> times (rDeliveryTimes.size() + 1);
            times.front() = dInitialTime;
            std::copy(rDeliveryTimes.begin(), rDeliveryTimes.end(), times.begin() + 1);

            // construct market forward price vector (t0 - tM)
            std::vector<double> forwardPricesLog (rForwardPrices.size() + 1);
            forwardPricesLog.front() = log(dSpot);
            std::transform(rForwardPrices.begin(), rForwardPrices.end(), forwardPricesLog.begin() + 1,
                           [](double price) { return log(price); });

        // perform interpolation (on logs)
        cfl::Interp rInterp = cfl::NInterp::linear(); // linear interpolation
        rInterp.assign(times.begin(), times.end(), forwardPricesLog.begin());

        return exp(rInterp.interp());
}