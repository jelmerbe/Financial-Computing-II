//
// Created by Jelmer Bennema on 3/3/24.
//
#include "Session4/Session4.hpp"

cfl::MultiFunction
prb::corridor(double dNotional, double dLowerBarrier, double dUpperBarrier, const std::vector<double> &rBarrierTimes,
              cfl::AssetModel &rModel) {

    // check preconditions/input data
            PRECONDITION(dLowerBarrier < dUpperBarrier);
            PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
            PRECONDITION(std::is_sorted(rBarrierTimes.begin(), rBarrierTimes.end()));

    // construct time vector
    std::vector<double> eventTimes(rBarrierTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    copy(rBarrierTimes.begin(), rBarrierTimes.end(), eventTimes.begin() + 1);
    rModel.assignEventTimes(eventTimes);

    auto iTime = eventTimes.size() - 1;
    cfl::Slice uOption = rModel.cash(iTime, 0.0), uSpot;

    // find final corridor payoff using spot price path
    while (iTime > 0) {
        uSpot = rModel.spot(iTime);
        uOption += cfl::indicator(uSpot, dLowerBarrier) * cfl::indicator(dUpperBarrier, uSpot)
                   * rModel.discount(iTime, eventTimes.back()); // view this as small discounted payout
        iTime--;
        uOption.rollback(iTime); // roll back to previous time
    }

    // return discounted payoff of corridor option
    uOption *= dNotional / rBarrierTimes.size();
    return interpolate(uOption);
}