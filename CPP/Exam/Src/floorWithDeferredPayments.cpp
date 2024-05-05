
#include "Exam/Exam.hpp"


// use similar function as in Session 5, but adjust for deferred payment by discounting

cfl::MultiFunction prb::floorWithDeferredPayments(const cfl::Data::CashFlow &rFloor, cfl::InterestRateModel &rModel) {

    std::vector<double> uEventTimes(rFloor.numberOfPayments);
    uEventTimes.front() = rModel.initialTime();
    std::transform(uEventTimes.begin(), uEventTimes.end() - 1,
                   uEventTimes.begin() + 1,
                   [&rFloor](double dX) { return dX + rFloor.period; });
    rModel.assignEventTimes(uEventTimes);

    // last minus one payment
    int iTime = uEventTimes.size() - 1;
    // floor payment plus notional (as percentage)
    double dFloorFactor = 1. + rFloor.rate * rFloor.period;
    double dPaymentTime = uEventTimes[iTime] + rFloor.period;
    cfl::Slice uDiscount = rModel.discount(iTime, dPaymentTime);
    // value of next payment (we multiply on notional at the end)
    cfl::Slice uOption = max( dFloorFactor - 1./uDiscount, 0.);
    double dMaturity = rModel.eventTimes().back();

    while (iTime > 0) {
        // uOption is the value of future payments
        iTime--;
        uOption.rollback(iTime);
        dPaymentTime = uEventTimes[iTime] + rFloor.period;
        uDiscount = rModel.discount(iTime, dPaymentTime);
        // this line is different from normal floor due to deferred payment (see session 5 algorithms)
        uOption += max( dFloorFactor - 1./uDiscount, 0.) * rModel.discount(iTime, dMaturity);
    }
    uOption *= rFloor.notional;
    return interpolate(uOption);
}