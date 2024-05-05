//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction
prb::boost(double dNotional, double dLowerBarrier, double dUpperBarrier, const std::vector<double> &rBarrierTimes,
           cfl::AssetModel &rModel) {

    // check preconditions
            PRECONDITION(dLowerBarrier < dUpperBarrier);

    // assign event times
    std::vector<double> times(rBarrierTimes.size() + 1);
    times.front() = rModel.initialTime();
    std::copy(rBarrierTimes.begin(), rBarrierTimes.end(), times.begin() + 1);

    rModel.assignEventTimes(times); // assign event times to model

    // set boundary conditions
    auto iTime = times.size() - 1;
    cfl::Slice uOption = rModel.cash(iTime, dNotional);
    cfl::Slice uTerminal, uInd;

    while (iTime > 0) {

        uTerminal = rModel.cash(iTime, dNotional * (iTime - 1)/ (times.size() - 1));

        uInd = cfl::indicator(dLowerBarrier, rModel.spot(iTime)) + cfl::indicator(rModel.spot(iTime), dUpperBarrier);
        uOption += (uTerminal - uOption) * uInd;

        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}