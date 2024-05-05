

#include "Exam/Exam.hpp"

cfl::MultiFunction prb::downRebateUpOut(double dNotional, double dLowerBarrier, double dUpperBarrier,
                                        const std::vector<double> &rLowerBarrierTimes,
                                        const std::vector<double> &rUpperBarrierTimes, cfl::AssetModel &rModel) {

    // check preconditions
            PRECONDITION(std::is_sorted(rLowerBarrierTimes.begin(), rLowerBarrierTimes.end()));
            PRECONDITION(std::is_sorted(rUpperBarrierTimes.begin(), rUpperBarrierTimes.end()));

    // create time vector that includes both upper and lower event times (merge them)
    std::vector<double> eventTimes(rLowerBarrierTimes.size() + rUpperBarrierTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    std::merge(rLowerBarrierTimes.begin(), rLowerBarrierTimes.end(), rUpperBarrierTimes.begin(),
               rUpperBarrierTimes.end(), eventTimes.begin() + 1);
    rModel.assignEventTimes(eventTimes); // assign event times to model

    // price option backwards in time
    size_t iTime = eventTimes.size() - 1; // time at maturity (last upper or lower barrier time)
    cfl::Slice uOption = rModel.cash(iTime, 0.0); // option payoff

    while (iTime > 0) {
        // lower barrier time
        if (std::find(rLowerBarrierTimes.begin(), rLowerBarrierTimes.end(), rModel.eventTimes()[iTime])
            != rLowerBarrierTimes.end()) {
            uOption += (dNotional - uOption) * cfl::indicator(dLowerBarrier, rModel.spot(iTime));
        }

        // upper barrier time
        if (std::find(rUpperBarrierTimes.begin(), rUpperBarrierTimes.end(), rModel.eventTimes()[iTime])
            != rUpperBarrierTimes.end()) {
            uOption *= cfl::indicator(dUpperBarrier, rModel.spot(iTime));
        }
        
        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}