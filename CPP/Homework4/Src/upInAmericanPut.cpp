//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Homework4/Homework4.hpp"
#include <iostream>

cfl::MultiFunction prb::upInAmericanPut(double dBarrier, const std::vector<double> &rBarrierTimes, double dStrike,
                                        const std::vector<double> &rExerciseTimes, cfl::AssetModel &rModel) {

    // check preconditions
            PRECONDITION(dBarrier > 0);
            PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
            PRECONDITION(rBarrierTimes.front() < rExerciseTimes.front());
            PRECONDITION(rBarrierTimes.back() < rExerciseTimes.back());

    // create event times
    std::vector<double> eventTimes(rBarrierTimes.size() + rExerciseTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    std::merge(rBarrierTimes.begin(), rBarrierTimes.end(), rExerciseTimes.begin(),
               rExerciseTimes.end(), eventTimes.begin() + 1);
    rModel.assignEventTimes(eventTimes); // assign event times to model

    // construct up-and-in american put function
    auto iTime = eventTimes.size() - 1; // time at maturity
    cfl::Slice uOption = rModel.cash(iTime, 0.0);
    cfl::Slice uPut = rModel.cash(iTime, 0.0);
    cfl::Slice uSpot;
    double dTime;

    while (iTime > 0) {
        dTime = eventTimes[iTime]; // actual time
        uSpot = rModel.spot(iTime);

        // if actual time is in exercise times, compute put value
        if (std::find(rExerciseTimes.begin(), rExerciseTimes.end(), dTime) != rExerciseTimes.end()) {
            uPut = max(uPut, max(dStrike - uSpot, 0.0));
        }

        // if actual time is in barrier times, compute option value
        if (std::find(rBarrierTimes.begin(), rBarrierTimes.end(), dTime) != rBarrierTimes.end()) {
            uOption += (uPut - uOption) * cfl::indicator( uSpot, dBarrier);
        }
        iTime--;
        uPut.rollback(iTime);
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}