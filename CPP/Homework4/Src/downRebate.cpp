//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Homework4/Homework4.hpp"

cfl::MultiFunction prb::downRebate(double dLowerBarrier, double dNotional, const std::vector<double> &rBarrierTimes,
                                   cfl::AssetModel &rModel) {
    // check preconditions
            PRECONDITION(dLowerBarrier > 0);
            PRECONDITION(std::is_sorted(rBarrierTimes.begin(), rBarrierTimes.end()));

    // create event times
    std::vector<double> eventTimes(rBarrierTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    std::copy(rBarrierTimes.begin(), rBarrierTimes.end(), eventTimes.begin() + 1);

    rModel.assignEventTimes(eventTimes); // assign event times to model

    // construct down-and-out rebate function
    auto iTime = eventTimes.size() - 1; // time at maturity
    cfl::Slice uOption = rModel.cash(iTime, 0.0); // if price never below barrier, payoff = 0
    cfl::Slice uSpot;

    while (iTime > 0) {
        uSpot = rModel.spot(iTime);

        uOption += (dNotional - uOption) * cfl::indicator(dLowerBarrier, uSpot); // update option value
        iTime--;
        uOption.rollback(iTime); // roll back to previous time
    }

    return interpolate (uOption);
}