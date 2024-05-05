//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction prb::americanChooser(const std::vector<double> &rPutStrikes, const std::vector<double> &rCallStrikes,
                                        const std::vector<double> &rExerciseTimes, double dMaturity,
                                        cfl::AssetModel &rModel) {

    // construct time vector
    std::vector<double> times(rExerciseTimes.size() + 2);
    times.front() = rModel.initialTime();
    std::copy(rExerciseTimes.begin(), rExerciseTimes.end(), times.begin() + 1);
    times.back() = dMaturity;
    rModel.assignEventTimes(times); // assign event times to model

    // terminal conditions
    auto iTime = rModel.eventTimes().size() - 1; // start at time T (maturity)
    cfl::Slice uOption = rModel.cash(iTime, 0);
    cfl::Slice uChoose, uCall, uPut;
    cfl::Slice uFinal = rModel.spot(iTime);

    while (iTime > 0) {
        // find put and call payoffs and discount to current time
        uCall = max(uFinal - rCallStrikes[iTime - 2], 0);
        uPut = max(rPutStrikes[iTime - 2] - uFinal, 0);
        uCall.rollback(iTime);
        uPut.rollback(iTime);

        uChoose = max(uCall,uPut);
        uOption = max(uOption, uChoose);
        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}
