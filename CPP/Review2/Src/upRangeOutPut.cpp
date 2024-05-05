//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction
prb::upRangeOutPut(double dUpperBarrier, unsigned int iOutTimes, const std::vector<double> &rBarrierTimes,
                   double dStrike, double dMaturity, cfl::AssetModel &rModel) {

    // standard european put option --

    // construct time vector
    std::vector<double> times(rBarrierTimes.size() + 2);
    times.front() = rModel.initialTime();
    std::copy(rBarrierTimes.begin(), rBarrierTimes.end(), times.begin() + 1);
    times.back() = dMaturity;
    rModel.assignEventTimes(times); // assign event times to model

    // terminal conditions
    auto iTime = rModel.eventTimes().size() - 1; // start at time T (maturity)
    std::vector<cfl::Slice> uOption(iOutTimes, max(dStrike - rModel.spot(iTime), 0.0));
    iTime--;
    for (int Ind = 0; Ind < iOutTimes; Ind++) {
        uOption[Ind].rollback(iTime);
    }

    // start loop at time t_N
    while (iTime > 0) {
        // check if barrier was crossed at this time
        cfl::Slice crossed = cfl::indicator(rModel.spot(iTime), dUpperBarrier);

        // if barrier was crossed, move zero to the left
        for (int Ind = 0; Ind + 1 < iOutTimes; Ind++) {
            uOption[Ind] += crossed * (uOption[Ind + 1] - uOption[Ind]);
        }

        // if barrier was crossed set last option to zero
        uOption.back() *= (1 - crossed);

        iTime--;
        // roll back all options
        for (int Ind = 0; Ind < iOutTimes; Ind++) {
            uOption[Ind].rollback(iTime);
        }
    }

    return interpolate(uOption.front());
}