//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction
prb::clique(double dMaturity, const std::vector<double> &rAverageTimes, double dStrike, cfl::AssetModel &rModel) {

    // check preconditions
    PRECONDITION(dMaturity > rAverageTimes.back());

    // construct time vector
    std::vector<double> times(rAverageTimes.size() + 2);
    times.front() = rModel.initialTime();
    std::copy(rAverageTimes.begin(), rAverageTimes.end(), times.begin() + 1);
    times.back() = dMaturity;

    rModel.assignEventTimes(times); // assign event times to model

    // find final payoff
    auto iTime = times.size() - 2; // start at t_M
    cfl::Slice uOption = rModel.cash(iTime, 0); // final payoff

    while (iTime > 0){
        uOption += max(rModel.spot(iTime) - dStrike, 0) * rModel.discount(iTime, dMaturity);

        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption / rAverageTimes.size());
}