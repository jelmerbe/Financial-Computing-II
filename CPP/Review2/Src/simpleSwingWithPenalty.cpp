//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction
prb::simpleSwingWithPenalty(double dStrike, double dPenalty, const std::vector<double> &rExerciseTimes,
                            double dMaturity, cfl::AssetModel &rModel) {

            PRECONDITION(dMaturity > rExerciseTimes.back());

    // construct time vector
    std::vector<double> times(rExerciseTimes.size() + 2);
    times.front() = rModel.initialTime();
    std::copy(rExerciseTimes.begin(), rExerciseTimes.end(), times.begin() + 1);
    times.back() = dMaturity;
    rModel.assignEventTimes(times); // assign event times to model

    auto iTime = rModel.eventTimes().size() - 1; // time at Maturity T
    cfl::Slice uOption = rModel.cash(iTime, -dPenalty); // final payoff

    while (iTime > 0) {
        iTime--;
        uOption.rollback(iTime);

        uOption = max((rModel.spot(iTime) - dStrike * rModel.discount(iTime, dMaturity)),
                      uOption);
    }

    return interpolate(uOption);
}