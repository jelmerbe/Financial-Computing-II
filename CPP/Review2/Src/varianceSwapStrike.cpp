//
// Created by Jelmer Bennema on 3/6/24.
//

#include "Review2/Review2.hpp"

cfl::MultiFunction prb::varianceSwapStrike(double dMaturity, unsigned int iNumberOfTimes, cfl::AssetModel &rModel) {

    double dPeriod = (dMaturity - rModel.initialTime()) / iNumberOfTimes;

    // construct time vector
    std::vector<double> eventTimes(iNumberOfTimes + 1);
    eventTimes.front() = rModel.initialTime();
    std::transform(eventTimes.begin(), eventTimes.end() - 1, eventTimes.begin() + 1,
                   [dPeriod](double dT) {
                       return dT + dPeriod;
                   });
    rModel.assignEventTimes(eventTimes); // assign event times to model

    // compute sum of variance
    auto iTime = eventTimes.size() - 1;
    cfl::Slice uVariance = rModel.cash(iTime, 0.0);
    cfl::Slice uSpot, uPast;
    cfl::Slice uSum = rModel.cash(iTime, 0.0);

    while (iTime > 0) {
        uSpot = rModel.spot(iTime);
        uPast = rModel.spot(iTime - 1);

        cfl::Slice uSum = log(uSpot) * log(uSpot);

        uVariance += log(uSpot / uPast) * log(uSpot / uPast)
                     * rModel.discount(iTime, dMaturity);

        iTime--;
        uVariance.rollback(iTime);
    }

    // find strike
    uVariance *= 1 / (dMaturity - rModel.initialTime()) * rModel.discount(0, dMaturity);

    return interpolate(uVariance);
}