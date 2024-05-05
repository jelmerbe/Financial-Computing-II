//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Homework4/Homework4.hpp"
#include <vector>

cfl::MultiFunction prb::straddle(double dStrike, double dMaturity, cfl::AssetModel &rModel) {

    // check preconditions
            PRECONDITION(dStrike > 0);
            PRECONDITION(rModel.initialTime() < dMaturity);

    // create event times (not path dependent)
    std::vector<double> eventTimes = {rModel.initialTime(), dMaturity};
    rModel.assignEventTimes(eventTimes);

    // find final payoff
    cfl::Slice uOption = abs(rModel.spot(1) - dStrike);
    uOption.rollback(0);

    return interpolate (uOption);
}
