

#include "Exam/Exam.hpp"

cfl::Function prb::forwardSwapRate(double dPeriod, unsigned int iNumberOfPayments, const cfl::Function &rDiscount) {

    // check preconditions
            PRECONDITION(dPeriod > 0.0);
            PRECONDITION(iNumberOfPayments > 0);

    // create function that computes forward swap rates for different start times
    std::function<double(double)> fSwap = [dPeriod, iNumberOfPayments, rDiscount](double dT) {
        double PV_Float, accrual_Factor = 0.0;
        double time;

        PV_Float = rDiscount(dT) - rDiscount(dT + dPeriod * iNumberOfPayments);
        for (unsigned int m = 1; m <= iNumberOfPayments; m++) {
            time = dT + dPeriod * m;
            accrual_Factor += dPeriod * rDiscount(time);
        }

        return PV_Float / accrual_Factor;
    };

    return cfl::Function(fSwap);
}
