//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Session4/Session4.hpp"
#include <iostream>

cfl::MultiFunction
prb::downOutCall(double dBarrier, const std::vector<double> &rBarrierTimes, double dStrike, double dMaturity,
                 cfl::AssetModel &rModel) {

            PRECONDITION(dMaturity > rBarrierTimes.back());
            PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
            PRECONDITION(std::is_sorted(rBarrierTimes.begin(), rBarrierTimes.end()));

    // construct time vector
    std::vector<double> eventTimes(rBarrierTimes.size() + 2);
    eventTimes.front() = rModel.initialTime();
    std::copy(rBarrierTimes.begin(), rBarrierTimes.end(), eventTimes.begin() + 1);
    eventTimes.back() = dMaturity;

    rModel.assignEventTimes(eventTimes); // assign event times to model

    // construct barrier function
    size_t iTime = eventTimes.size() - 1; // time at maturity
    cfl::Slice uOption = max(rModel.spot(iTime) - dStrike, 0.0); // option payoff

    // use for loop to price option
    while (iTime > 0) {
        uOption *= cfl::indicator(rModel.spot(iTime), dBarrier); // barrier indicator
        iTime--;
        uOption.rollback(iTime); // roll back to previous time
    }

    return interpolate(uOption); // return option price and sensitivity
}