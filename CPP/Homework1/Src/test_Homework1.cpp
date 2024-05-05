#include "Homework1/Homework1.hpp"
#include "Homework1/Output.hpp"
#include "test/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

void
volatilityVar ()
{
  test::print ("VOLATILITY CURVE FROM VARIANCE CURVE");

  double dInitialTime = 0.75;
  print (dInitialTime, "initial time", true);

  double dSigma = 0.25;

  Function uVol (
      [dSigma, dInitialTime] (double dT) {
        return dSigma * (1. + (dT - dInitialTime));
      },
      dInitialTime);

  Function uVar (
      [dInitialTime, uVol] (double dT) {
        double dSigma = uVol (dT);

        return dSigma * dSigma * (dT - dInitialTime);
      },
      dInitialTime);

  Function uV = prb::volatilityVar (uVar, dInitialTime);
  Function uErr = cfl::abs (uV - uVol);

  double dInterval = 1.;
  test::Data::print ("The result:", uV, uErr, dInitialTime, dInterval);
}

void
carryBlack ()
{
  test::print ("COST-OF-CARRY RATE IN BLACK MODEL");

  double dTheta = 0.03;
  double dLambda = 0.05;
  double dSigma = 0.2;
  double dInitialTime = 0.75;

  print (dTheta, "theta");
  print (dLambda, "lambda");
  print (dSigma, "sigma");
  print (dInitialTime, "initial time", true);

  Function uCostOfCarry
      = prb::carryBlack (dTheta, dLambda, dSigma, dInitialTime);
  double dInterval = 1;
  test::Data::print (uCostOfCarry, dInitialTime, dInterval);
}

void
swapRate ()
{
  test::print ("SWAP RATE");

  double dRate = 0.03;
  double dInitialTime = 1.5;
  double dPeriod = 0.25;
  unsigned iNumberOfPayments = 4;

  print (dPeriod, "swap period");
  print (iNumberOfPayments, "number of payments");
  print (dInitialTime, "initial time", true);

  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  uDiscount += cfl::Data::discount (dRate * 4, dInitialTime);
  uDiscount *= 0.5;

  double dSwapRate
      = prb::swapRate (dPeriod, iNumberOfPayments, uDiscount, dInitialTime);
  print (dSwapRate, "swap rate", true);
}

void
forwardCashFlow ()
{
  test::print ("FORWARD PRICES FOR A CASH FLOW");

  double dRate = test::c_dYield;
  double dInitialTime = 1.;
  unsigned iPayments = 6;
  std::vector<double> uPayments (iPayments);
  uPayments.front () = 100.;
  std::transform (uPayments.begin (), uPayments.end () - 1,
                  uPayments.begin () + 1,
                  [] (double dX) { return dX * (1. + 0.5 * std::cos (dX)); });
  std::vector<double> uTimes
      = getTimes (dInitialTime + 0.5, dInitialTime + 3., iPayments);
  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  test::print (dRate, "interest rate");
  test::print (dInitialTime, "initial time", true);
  test::Data::print ("cash flow:", uTimes, uPayments);
  Function uForwardCashFlow
      = prb::forwardCashFlow (uPayments, uTimes, uDiscount, dInitialTime);
  double dInterval = (uTimes.back () - dInitialTime) / 1.01;
  test::Data::print (uForwardCashFlow, dInitialTime, dInterval);
}

std::function<void ()>
test_Homework1 ()
{
  return [] () {
    print ("DATA CURVES FOR FINANCIAL MODELS");
    volatilityVar ();
    carryBlack ();
    swapRate ();
    forwardCashFlow ();
  };
}

int
main ()
{
  project (test_Homework1 (), PROJECT_NAME, PROJECT_NAME, "Homework 1");
}
