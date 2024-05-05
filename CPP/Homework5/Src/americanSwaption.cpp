//
// Created by Jelmer Bennema on 3/4/24.
//

#include "Homework5/Homework5.hpp"


// function to price the swap at given time (iTime)
cfl::Slice swap(unsigned int iTime, const cfl::Data::Swap &rSwap, cfl::InterestRateModel &rModel) {

    cfl::Slice uFixed = rModel.cash(iTime, 0.0);
    double dTime = rModel.eventTimes()[iTime];

    for (int k = 0; k < rSwap.numberOfPayments; k++) {
        dTime += rSwap.period;
        uFixed += rSwap.rate * rSwap.period * rModel.discount(iTime, dTime);
    }
    uFixed *= rSwap.notional;
    cfl::Slice uFloat = (1 - rModel.discount(iTime, dTime)) * rSwap.notional;

    // assume first we receive fixed and pay float
    cfl::Slice uSwap = uFixed - uFloat;
    if (!rSwap.payFloat) { uSwap *= -1; }

    return uSwap;
}


cfl::MultiFunction prb::americanSwaption(const cfl::Data::Swap &rSwap, const std::vector<double> &rExerciseTimes,
                                         cfl::InterestRateModel &rModel) {

    // construct time vector
    std::vector<double> eventTimes(rExerciseTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    std::copy(rExerciseTimes.begin(), rExerciseTimes.end(), eventTimes.begin() + 1);
    rModel.assignEventTimes(eventTimes);

    // boundary conditions
    auto iTime = eventTimes.size() - 1; // maturity
    cfl::Slice uOption = rModel.cash(iTime, 0.0);

    // price backwards in time
    while (iTime > 0) {

        uOption = max(swap(iTime, rSwap, rModel), uOption);
        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}