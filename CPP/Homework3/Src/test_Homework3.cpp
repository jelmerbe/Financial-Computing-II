#include "Homework3/Homework3.hpp"
#include "Homework3/Output.hpp"
#include "test/Data.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

const std::string c_sVol ("Fitted volatilities and their errors:");
const std::string c_sDF ("Fitted discount factors and their errors:");
const std::string c_sConstYield ("We fit with constant yield.");
const std::string c_sForward ("Fitted forward prices and their errors:");
const std::string c_sConstCarry ("We fit with constant cost-of-carry rate.");
const std::string
    c_sForwardFX ("Fitted forward exchange rates and their errors:");

void
discountSvenssonFit ()
{
  test::print ("LEAST-SQUARES FIT OF DISCOUNT CURVE IN SVENSSON MODEL");

  double dLambda1 = 0.02;
  double dLambda2 = 0.1;
  double dInitialTime = 1.;

  print (dLambda1, "lambda1");
  print (dLambda2, "lambda2");
  auto uDF = test::Data::getDiscount (dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uDiscount = prb::discountSvenssonFit (
      uDF.first, uDF.second, dLambda1, dLambda2, dInitialTime, uErr, uParam);
  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
discountRateFit ()
{
  test::print ("LEAST-SQUARES FIT OF DISCOUNT CURVE TO FLOAT RATES");

  double dInitialTime = 1.;

  auto uL = test::Data::getRate (dInitialTime);

  print (c_sConstYield);
  cfl::Fit uFit = cfl::NFit::linear (cfl::Function (1.));

  Function uErr;
  Function uDiscount
      = prb::discountRateFit (uL.first, uL.second, dInitialTime, uFit, uErr);

  double dInterval = uL.first.back () / 1.1;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
forwardFXCarryFit ()
{
  test::print ("FORWARD PRICES FOR EXCHANGE RATES BY LEAST-SQUARES FIT");

  double dSpotFX = 100;
  double dInitialTime = 1.;

  auto uDF = test::Data::getDiscountFX (dSpotFX, dInitialTime);

  print (c_sConstCarry);
  Fit uFit = NFit::linear (Function (1.));
  Function uErr;

  Function uForwardFX
      = prb::forwardFXCarryFit (dSpotFX, get<0> (uDF), get<1> (uDF),
                                get<2> (uDF), dInitialTime, uFit, uErr);

  double dInterval = get<0> (uDF).back () - dInitialTime;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sForwardFX, uForwardFX, uErr, dInitialTime, dInterval);
}

void
volatilityBlackFit ()
{
  test::print ("LEAST-SQUARES FIT OF VOLATILITY CURVE FOR BLACK MODEL");

  double dLambda = 0.05;
  double dInitialTime = 1.;

  print (dLambda, "lambda");
  auto uVol = test::Data::getVol (dInitialTime);

  FitParam uParam;
  Function uErr;
  Function uVolat = prb::volatilityBlackFit (uVol.first, uVol.second, dLambda,
                                             dInitialTime, uErr, uParam);

  double dInterval = uVol.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sVol, uVolat, uErr, dInitialTime, dInterval);
}

std::function<void ()>
test_Homework3 ()
{
  return [] () {
    print ("LEAST SQUARES FIT OF DATA CURVES");

    discountSvenssonFit ();
    discountRateFit ();
    forwardFXCarryFit ();
    volatilityBlackFit ();
  };
}

int
main ()
{
  project (test_Homework3 (), PROJECT_NAME, PROJECT_NAME, "Homework 3");
}
