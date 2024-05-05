//
// Created by Jelmer Bennema on 3/3/24.
//

#include "Homework4/Homework4.hpp"

cfl::MultiFunction
prb::americanCallOnForward(double dForwardPrice, double dTimeToMaturity, const std::vector<double> &rExerciseTimes,
                           cfl::AssetModel &rModel) {

    // check preconditions
            PRECONDITION((dForwardPrice > 0) && (dTimeToMaturity > 0));
            PRECONDITION(std::is_sorted(rExerciseTimes.begin(), rExerciseTimes.end()));

    // construct time vector
    std::vector<double> eventTimes(rExerciseTimes.size() + 1);
    eventTimes.front() = rModel.initialTime();
    copy(rExerciseTimes.begin(), rExerciseTimes.end(), eventTimes.begin() + 1);

    rModel.assignEventTimes(eventTimes); // assign event times to model

    // payoff at maturity
    auto iTime = eventTimes.size() - 1;
    double dMaturity =  eventTimes[iTime] + dTimeToMaturity;
    cfl::Slice uOption = max(rModel.discount(iTime, dMaturity) *  (rModel.forward(iTime, eventTimes[iTime]
                                                   + dTimeToMaturity) - dForwardPrice), 0.0);
    cfl::Slice uCallOnForward, forward_price;


    while (iTime > 0) {
        dMaturity = eventTimes[iTime] + dTimeToMaturity;
        forward_price = rModel.discount(iTime, dMaturity) *
                        (rModel.forward(iTime, dMaturity) - dForwardPrice);
        uCallOnForward = max(forward_price, 0.0);
        uOption = max(uOption, uCallOnForward);

        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}