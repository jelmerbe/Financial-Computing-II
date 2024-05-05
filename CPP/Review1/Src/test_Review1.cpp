#include "Review1/Output.hpp"
#include "Review1/Review1.hpp"
#include "test/Black.hpp"
#include "test/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

// DATA CURVES

void
yieldVasicek ()
{
  test::print ("YIELD CURVE IN VASICEK MODEL");

  double dLambda = 0.05;
  double dTheta = 0.02;
  double dR0 = 0.04;
  double dSigma = 0.01;
  double dInitialTime = 1.5;

  print (dTheta, "theta");
  print (dLambda, "lambda");
  print (dSigma, "sigma");
  print (dR0, "r_0");
  print (dInitialTime, "initial time", true);

  Function uYield
      = prb::yieldVasicek (dTheta, dLambda, dSigma, dR0, dInitialTime);
  double dInterval = 5;
  test::Data::print (uYield, dInitialTime, dInterval);
}

void
forwardAnnuity ()
{
  test::print ("FORWARD PRICES FOR AN ANNUITY");

  cfl::Data::CashFlow uAnnuity = test::swapParameters ();
  uAnnuity.notional = 1;
  double dRate = uAnnuity.rate;
  double dInitialTime = 1.;
  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);

  print (dRate, "interest rate");
  print (dInitialTime, "initial time", true);

  test::printCashFlow (uAnnuity, "annuity parameters");

  for (int iI = 0; iI < 2; iI++)
    {
      bool bClean = (iI == 0) ? true : false;
      if (bClean)
        {
          print ("clean prices:");
        }
      else
        {
          print ("dirty prices:");
        }
      double dRate = uAnnuity.rate;
      double dPeriod = uAnnuity.period;
      double dMaturity = dInitialTime + dPeriod * uAnnuity.numberOfPayments;
      Function uForwardAnnuity = prb::forwardAnnuity (
          dRate, dPeriod, dMaturity, uDiscount, dInitialTime, bClean);
      double dInterval = uAnnuity.period * uAnnuity.numberOfPayments / 1.1;
      test::Data::print (uForwardAnnuity, dInitialTime, dInterval);
    }
}

// ROOTS

void
yieldTMAnnuity ()
{
  test::print ("YIELD TO MATURITY FOR AN ANNUITY WITH cfl::RootD");

  cfl::Data::CashFlow uAnnuity = test::swapParameters ();
  uAnnuity.notional = 1;
  double dInitialTime = 1.;
  double dPrice = uAnnuity.notional * uAnnuity.numberOfPayments * uAnnuity.rate
                  * uAnnuity.period / 1.1;
  double dRate = uAnnuity.rate;
  double dPeriod = uAnnuity.period;
  double dMaturity = dInitialTime + dPeriod * uAnnuity.numberOfPayments;
  double dY0 = 0.;
  double dErrR = 10E-6;
  double dErrA = 10E-8;

  test::printCashFlow (uAnnuity, "annuity parameters");

  print (dMaturity, "maturity");
  test::print (dPrice, "price");
  test::print (dInitialTime, "initial time");
  test::print (dY0, "initial guess for YTM");
  print (dErrR, "relative yield error");
  print (dErrA, "absolute yield error");
  print ("method = secant", true);

  RootD uRoot = cfl::NRootD::secant (dErrR, dErrA);

  double dYTM = prb::yieldTMAnnuity (dRate, dPeriod, dMaturity, dInitialTime,
                                     dPrice, dY0, uRoot);
  test::print (dYTM, "YTM", true);
}

void
forwardYTMAnnuity ()
{
  test::print ("FORWARD YIELD TO MATURITY FOR AN ANNUITY WITH cfl::RootD");

  cfl::Data::CashFlow uAnnuity = test::swapParameters ();
  uAnnuity.notional = 1;
  double dInitialTime = 1.;
  Function uDiscount = test::Data::getDiscountCurve (dInitialTime);
  double dErrR = 10E-6;
  double dErrA = 10E-8;
  RootD uRoot = cfl::NRootD::secant (dErrR, dErrA);

  test::printCashFlow (uAnnuity, "annuity parameters");

  print (dInitialTime, "initial time");
  print (dErrR, "relative yield error");
  print (dErrA, "absolute yield error");
  print ("method = secant", true);

  double dRate = uAnnuity.rate;
  double dPeriod = uAnnuity.period;
  double dMaturity = dInitialTime + dPeriod * uAnnuity.numberOfPayments;
  Function uForwardYTMAnnuity = prb::forwardYTMAnnuity (
      dRate, dPeriod, dMaturity, uDiscount, dInitialTime, uRoot);
  double dInterval = uAnnuity.period * uAnnuity.numberOfPayments / 1.1;
  test::Data::print (uForwardYTMAnnuity, dInitialTime, dInterval);
}

// INTERPOLATION

void
forwardLogLinInterp ()
{
  test::print ("LOG LINEAR INTERPOLATION OF FORWARD CURVE");

  double dSpot = 100;
  double dInitialTime = 1.;

  auto uF = test::Data::getForward (dSpot, dInitialTime);

  Function uForward
      = prb::forwardLogLinInterp (dSpot, uF.first, uF.second, dInitialTime);

  double dInterval = uF.first.back () - dInitialTime;
  test::Data::print (uForward, dInitialTime, dInterval);
}

void
forwardFXCarryLinInterp ()
{
  test::print (
      "FORWARD EXCHANGE CURVE BY LINEAR INTERPOLATION OF COST-OF-CARRY RATES");

  double dSpotFX = 100;
  double dInitialTime = 1.;

  auto uDF = test::Data::getDiscountFX (dSpotFX, dInitialTime);

  Function uForwardFX = prb::forwardFXCarryLinInterp (
      dSpotFX, get<0> (uDF), get<1> (uDF), get<2> (uDF), dInitialTime);

  double dInterval = get<0> (uDF).back () - dInitialTime;
  test::Data::print (uForwardFX, dInitialTime, dInterval);
}

// FIT

const std::string c_sForward ("Fitted forward prices and their errors:");
const std::string c_sDF ("Fitted discount factors and their errors:");
const std::string c_sConstYield ("We fit with constant yield.");

void
forwardBlack2Fit ()
{
  test::print ("LEAST-SQUARES FIT OF FORWARD CURVE IN 2-FACTOR BLACK MODEL");

  double dSpot = test::c_dSpot;
  double dLambda1 = test::Black::c_dLambda;
  double dSigma1 = test::Black::c_dSigma;
  double dSigma0 = dSigma1 * 0.5;
  double dInitialTime = test::c_dInitialTime;

  print (dLambda1, "lambda1");
  print (dSigma0, "sigma0");
  print (dSigma1, "sigma1");
  auto uF = test::Data::getForward (dSpot, dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uForward
      = prb::forwardBlack2Fit (dSpot, uF.first, uF.second, dLambda1, dSigma0,
                               dSigma1, dInitialTime, uErr, uParam);

  double dInterval = uF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sForward, uForward, uErr, dInitialTime, dInterval);
}

void
discountSwapFit1 ()
{
  test::print ("DISCOUNT CURVE OBTAINED FROM SWAP RATES BY LEAST-SQUARES FIT");

  double dPeriod = 1. / 12;
  double dInitialTime = 1.;

  std::vector<double> uSwapRates
      = test::Data::getSwapRates (dPeriod, dInitialTime);

  print (c_sConstYield);
  Fit uFit = cfl::NFit::linear (Function (1.));

  Function uErr;

  Function uDiscount
      = prb::discountSwapFit (uSwapRates, dPeriod, dInitialTime, uFit, uErr);

  double dInterval = uSwapRates.size () * dPeriod / 1.1;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime + 0.001, dInterval);
}

std::function<void ()>
test_Review1 ()
{
  return [] () {
    print ("DATA CURVES FOR FINANCIAL MODELS");

    yieldVasicek ();
    forwardAnnuity ();

    print ("ROOT FINDING");

    yieldTMAnnuity ();
    forwardYTMAnnuity ();

    print ("INTERPOLATION OF DATA CURVES");

    forwardLogLinInterp ();
    forwardFXCarryLinInterp ();

    print ("FITTING OF DATA CURVES");

    forwardBlack2Fit ();
    discountSwapFit1 ();
  };
}

int
main ()
{
  project (test_Review1 (), PROJECT_NAME, PROJECT_NAME, "Review 1");
}
