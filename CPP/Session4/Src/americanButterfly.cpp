//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Session4/Session4.hpp"

cfl::MultiFunction prb::americanButterfly(double dStrike, double dStrikeStep, const std::vector<double> &rExerciseTimes,
                                          cfl::AssetModel &rModel) {

    // CHECK PRECONDITIONS
            PRECONDITION(std::is_sorted(rExerciseTimes.begin(), rExerciseTimes.end()));
            PRECONDITION(rModel.initialTime() < rExerciseTimes.front());
            PRECONDITION(dStrikeStep < dStrike);

    // construct time vector
    std::vector<double> eventTimes(rExerciseTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    std::copy(rExerciseTimes.begin(), rExerciseTimes.end(), eventTimes.begin() + 1);

    rModel.assignEventTimes(eventTimes); // assign event times to model

    // construct butterfly components (call options)
    auto iTime = eventTimes.size() - 1; // time at maturity

    // construct butterfly function
    cfl::Slice uOption = rModel.cash(iTime, 0.0); // if not exercised at maturity, receive zero
    cfl::Slice uSpot, uButterFly;

    while (iTime > 0) {
        uSpot = rModel.spot(iTime);
        uButterFly = max(uSpot - (dStrike + dStrikeStep), 0.0)
                     - 2 * max(uSpot - dStrike, 0.0)
                     + max(uSpot - (dStrike - dStrikeStep), 0.0);

        uOption = max(uOption, uButterFly); // update option value
        iTime--;
        uOption.rollback(iTime); // roll back to previous time
    }

    return interpolate (uOption);
}