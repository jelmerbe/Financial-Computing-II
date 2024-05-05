//
// Created by Jelmer Bennema on 3/4/24.
//

#include "Session5/Session5.hpp"
#include <iostream>

cfl::MultiFunction prb::floor(const cfl::Data::CashFlow &rFloor, cfl::InterestRateModel &rModel) {

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
    cfl::Slice uOption = max(uDiscount * dFloorFactor - 1., 0.);


    while (iTime > 0) {
        // uOption is the value of future payments
        iTime--;
        uOption.rollback(iTime);
        dPaymentTime = uEventTimes[iTime] + rFloor.period;
        uDiscount = rModel.discount(iTime, dPaymentTime);

        uOption += max(uDiscount * dFloorFactor - 1., 0.);
    }
    uOption *= rFloor.notional;
    return interpolate(uOption);
}


cfl::MultiFunction
prb::putOnFRA(double dFixedRate, double dPeriod, double dNotional, double dMaturity, cfl::InterestRateModel &rModel) {

    // construct time vector
    std::vector<double> vTimes = {rModel.initialTime(), dMaturity};
    rModel.assignEventTimes(vTimes); // assign event times to model

    auto iTime = 1; // time at maturity

    cfl::Slice uFRA = max(1 - (1 + dFixedRate * dPeriod)
                              * rModel.discount(iTime, dMaturity + dPeriod), 0.0);
    uFRA *= dNotional;

    //discount back to initial time
    uFRA.rollback(0);

    return interpolate(uFRA);
}

// function to find constant swap rate
cfl::Slice swapRate(unsigned int iTime, double dPeriod,
                    unsigned int iSwapPayments, cfl::InterestRateModel &rModel) {

    double dTime = rModel.eventTimes()[iTime];
    cfl::Slice uFloat = 1. - rModel.discount(iTime, dTime + dPeriod * iSwapPayments);
    cfl::Slice uFixed = rModel.cash(iTime, 0.0);

    for (int l = 0; l < iSwapPayments; l++) {
        dTime += dPeriod;
        uFixed += dPeriod * rModel.discount(iTime, dTime);
    }

    cfl::Slice uRate = uFloat / uFixed;

    return uRate;
}



cfl::MultiFunction prb::capOnSwapRate(const cfl::Data::CashFlow &rCap, double dSwapPeriod, unsigned int iSwapPayments,
                                      cfl::InterestRateModel &rModel) {

    // event times: initial time + payment times − maturity
    std::vector<double> vTimes (rCap.numberOfPayments); vTimes.front () = rModel.initialTime ();
    std::transform (vTimes.begin (), vTimes.end () - 1,
            vTimes.begin () + 1,
            [&rCap] (double dX) { return dX + rCap.period; });
    rModel.assignEventTimes (vTimes);

    // last minus one payment time
    int iTime = vTimes.size () - 1;
    cfl::Slice uDiscount
            = rModel.discount (iTime, rModel.eventTimes ()[iTime] + rCap.period);
    cfl::Slice uSwapRate = swapRate (iTime, dSwapPeriod, iSwapPayments, rModel);
    cfl::Slice uOption = uDiscount * max (uSwapRate - rCap.rate, 0.);
    while (iTime > 0) {

        iTime--;
        uOption.rollback (iTime);
        uDiscount
                = rModel.discount (iTime, rModel.eventTimes ()[iTime] + rCap.period);
        uSwapRate = swapRate (iTime, dSwapPeriod, iSwapPayments, rModel);
        uOption += uDiscount * max (uSwapRate - rCap.rate, 0.);
    }
    uOption *= rCap.notional * rCap.period;
    return interpolate (uOption);
}

cfl::MultiFunction prb::cancelSwapArrears(const cfl::Data::Swap &rSwap, cfl::InterestRateModel &rModel) {


}
