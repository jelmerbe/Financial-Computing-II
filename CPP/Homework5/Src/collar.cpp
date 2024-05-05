//
// Created by Jelmer Bennema on 3/4/24.
//

#include "Homework5/Homework5.hpp"

cfl::MultiFunction prb::collar(const cfl::Data::CashFlow &rCap, double dFloorRate, cfl::InterestRateModel &rModel) {

    // check preconditions
            PRECONDITION(rCap.rate > dFloorRate);

    // construct time vector
    std::vector<double> eventTimes(rCap.numberOfPayments);
    eventTimes.front() = rModel.initialTime();
    std::transform(eventTimes.begin(), eventTimes.end() - 1, eventTimes.begin() + 1,
                   [&rCap](double t) { return t + rCap.period; });
    rModel.assignEventTimes(eventTimes); // assign event times to model

    // find collar pay off
    auto iTime = eventTimes.size() - 1;
    cfl::Slice uDiscount =
            rModel.discount(iTime, rModel.eventTimes()[iTime] + rCap.period);
    double dCapFactor = 1 + rCap.rate * rCap.period;
    double dFloorFactor = 1 + dFloorRate * rCap.period;
    cfl::Slice uCap = max(1 - uDiscount * dCapFactor, 0);
    cfl::Slice uFloor = max(uDiscount * dFloorFactor - 1, 0);
    cfl::Slice uOption = uCap - uFloor; // collar pay off

    while (iTime > 0) {
        iTime--;
        uOption.rollback(iTime);

        uDiscount = rModel.discount(iTime, rModel.eventTimes()[iTime] + rCap.period);
        uCap = max(1 - uDiscount * dCapFactor, 0);
        uFloor = max(uDiscount * dFloorFactor - 1, 0);
        uOption += (uCap - uFloor); // collar pay off
    }

    uOption *= rCap.notional;
    return interpolate(uOption);
}