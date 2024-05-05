//
// Created by Jelmer Bennema on 2/22/24.
//

#include "Session1/Session1.hpp"
#include <vector>
#include <numeric>

/* write approximations in case of 0/0 singularity.
 it takes in 2 * dLambda * (T - t) and checks for values */
double shape1(double dX) {
    if (dX > cfl::TIME_EPS) {
        return (1 - exp(-dX)) / dX;
    } else {
        return 1 - dX / 2 + dX * dX / 6;
    }
}

double shape2(double dX) {
    if (dX > cfl::EPS) {
        return (1 - exp(-dX)) / dX - exp(-dX);
    } else {
        return dX / 2 - dX * dX / 3;
    }
}

cfl::Function prb::volatilityBlack(double dSigma, double dLambda, double dInitialTime) {

            PRECONDITION((dLambda >= 0) && (dSigma > 0));

    // std::function<double(double)> Black;
    std::function<double(double)> Black;

    // lambda construction of Black function
    Black = [dSigma, dLambda, dInitialTime](double dT) {
                ASSERT(dT >= dInitialTime);

        double dX = 2 * dLambda * (dT - dInitialTime);
        return dSigma * sqrt(shape1(dX));
    };

    // return cfl function (taking in std::function, lower bound, upper bound)
    return cfl::Function(Black, dInitialTime);
}


cfl::Function prb::yieldNelsonSiegel(double dC0, double dC1, double dC2, double dLambda, double dInitialTime) {

            PRECONDITION(dLambda > 0);

    // create std function - lambda construction
    std::function<double(double)> Nelson
            = [dC0, dC1, dC2, dLambda, dInitialTime](double dT) {
                        ASSERT(dT >= dInitialTime);
                double dX = dLambda * (dT - dInitialTime);

                return dC0 + dC1 * shape1(dX) + dC2 * shape2(dX);
            };
    return cfl::Function(Nelson, dInitialTime);

}


cfl::Function prb::forwardRate(double dPeriod, const cfl::Function &rDiscount) {
            PRECONDITION(dPeriod > 0);
    std::function<double(double)> Rf
            = [dPeriod, rDiscount](double dT) {
                return rDiscount(dT) / (rDiscount(dT + dPeriod) * dPeriod) - 1 / dPeriod;

            };

    return cfl::Function(Rf);
}

cfl::Function prb::forwardStockDividends(double dSpot, const std::vector<double> &rDividendsTimes,
                                         const std::vector<double> &rDividends, const cfl::Function &rDiscount,
                                         double dInitialTime) {

    // pre-conditions
            PRECONDITION(rDividendsTimes.front() > dInitialTime);
            PRECONDITION(rDividendsTimes.size() == rDividends.size());
            PRECONDITION(std::is_sorted(rDividendsTimes.begin(), rDividendsTimes.end(),
                                        std::less_equal<>()));

    // create lambda std::function
    std::function<double(double)> forward
            = [dSpot, rDividendsTimes, rDividends, rDiscount, dInitialTime](double dT) {

                // iterator that points at first element in time vector
                std::vector<double>::const_iterator time;
                time = std::lower_bound(rDividendsTimes.begin(), rDividendsTimes.end(), dT);

                double dF = std::inner_product(
                        rDividendsTimes.begin(), time, rDividends.begin(), dSpot, std::minus<>(),
                        [rDiscount](double dT, double dD) { return dD * rDiscount(dT); });

                dF /= rDiscount(dT);

                return dF;
            };

    // load lambda function for specific dT into cfl function
    return cfl::Function(forward, dInitialTime, rDividendsTimes.back());
}

