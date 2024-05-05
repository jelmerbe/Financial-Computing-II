//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Session4/Session4.hpp"
#include <vector>

cfl::MultiFunction
prb::callOnForwardPrice(double dStrike, double dCallMaturity, double dForwardMaturity, cfl::AssetModel &rModel) {

    // check preconditions
    PRECONDITION(dCallMaturity < dForwardMaturity);
    PRECONDITION(rModel.initialTime() < dCallMaturity);

    // find final payoff
    auto eventTimes = rModel.eventTimes();

    auto iTime = std::distance(eventTimes.begin(), std::find(eventTimes.begin(), eventTimes.end(), dCallMaturity));
    cfl::Slice uOption = max(rModel.forward(iTime, dForwardMaturity) - dStrike, 0.0);

    // return discounted payoff
    uOption.rollback(0); // roll back to initial time
    return interpolate (uOption); // return option price and sensitivity
}

