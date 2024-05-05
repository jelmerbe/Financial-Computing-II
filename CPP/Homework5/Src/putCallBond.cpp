//
// Created by Jelmer Bennema on 3/4/24.
//

#include "Homework5/Homework5.hpp"

cfl::MultiFunction
prb::putCallBond(const cfl::Data::CashFlow &rCouponBond, double dRedemptionPrice, double dRepurchasePrice,
                 cfl::InterestRateModel &rModel) {

    // check preconditions

    // construct time vector
    std::vector<double> eventTimes(rCouponBond.numberOfPayments);
    eventTimes.front() = rModel.initialTime();
    std::transform(eventTimes.begin(), eventTimes.end() - 1, eventTimes.begin() + 1, [&rCouponBond](double dT) {
        return dT + rCouponBond.period;
    });
    rModel.assignEventTimes(eventTimes); // assign times to model

    // boundary conditions
    auto iTime = eventTimes.size() - 1;
    double couponPayment = rCouponBond.notional * rCouponBond.rate * rCouponBond.period;
    double redemption = dRedemptionPrice * rCouponBond.notional;
    double repurchase = dRepurchasePrice * rCouponBond.notional;

    cfl::Slice uOption = rModel.discount(iTime, rModel.eventTimes()[iTime] + rCouponBond.period)
                         * (couponPayment + rCouponBond.notional);

    // price backwards in time
    while (iTime > 0) {

        // repurchase
        uOption = min(repurchase, uOption);
        // redemption
        uOption = max(redemption, uOption);
        // coupon
        uOption += couponPayment;

        iTime--;
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}
