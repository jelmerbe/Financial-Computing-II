#include "Session1/Output.hpp"
#include "Session1/Session1.hpp"
#include "test/Data.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

void
volatilityBlack ()
{
  test::print ("STATIONARY IMPLIED VOLATILITY IN BLACK MODEL");
  double dLambda = 0.05;
  double dSigma = 0.2;
  double dInitialTime = 0.75;

  print (dSigma, "sigma");
  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);

  Function uVol = prb::volatilityBlack (dSigma, dLambda, dInitialTime);
  double dInterval = 1;
  test::Data::print (uVol, dInitialTime, dInterval);
}

void
yieldNelsonSiegel ()
{
  test::print ("NELSON-SIEGEL YIELD CURVE");

  double dLambda = 0.05;
  double dC0 = 0.02;
  double dC1 = 0.04;
  double dC2 = 0.06;
  double dInitialTime = 1.5;

  print (dC0, "c0");
  print (dC1, "c1");
  print (dC2, "c2");
  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);

  Function uYield
      = prb::yieldNelsonSiegel (dC0, dC1, dC2, dLambda, dInitialTime);
  double dInterval = 5;
  test::Data::print (uYield, dInitialTime, dInterval);
}

void
forwardRate ()
{
  test::print ("FORWARD RATES");

  double dRate = 0.03;
  double dInitialTime = 1.5;
  double dPeriod = 0.25;

  print (dPeriod, "rate period");
  print (dInitialTime, "initial time", true);

  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  uDiscount += cfl::Data::discount (dRate * 4, dInitialTime);
  uDiscount *= 0.5;

  Function uForwardRate = prb::forwardRate (dPeriod, uDiscount);

  double dInterval = 5;
  test::Data::print (uForwardRate, dInitialTime, dInterval);
}

void
forwardStockDividends ()
{
  test::print ("FORWARD PRICES FOR A STOCK WITH DIVIDENDS");

  double dSpot = 100;
  double dRate = 0.12;
  double dInitialTime = 1.;
  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  unsigned iTimes = 10;
  std::vector<double> uTimes (iTimes);
  double dPeriod = 0.5;
  uTimes.front () = dInitialTime + dPeriod;
  std::transform (uTimes.begin (), uTimes.end () - 1, uTimes.begin () + 1,
                  [dPeriod] (double dX) { return dX + dPeriod; });
  double dDividend = 5.;
  double dStep = 0.5;
  std::vector<double> uDividends (iTimes, dDividend);
  std::transform (uDividends.begin (), uDividends.end () - 1,
                  uDividends.begin () + 1,
                  [dStep] (double dD) { return dD + dStep; });

  print (dInitialTime, "initial time");
  print (dRate, "interest rate");
  print (dSpot, "spot", true);
  test::Data::print ("Stock dividends:", uTimes, uDividends);

  Function uForwardStockDividends = prb::forwardStockDividends (
      dSpot, uTimes, uDividends, uDiscount, dInitialTime);
  double dInterval = iTimes * dPeriod / 1.01;
  test::Data::print (uForwardStockDividends, dInitialTime, dInterval);
}

std::function<void ()>
test_Session1 ()
{
  return [] () {
    print ("DATA CURVES FOR FINANCIAL MODELS");

    volatilityBlack ();
    yieldNelsonSiegel ();
    forwardRate ();
    forwardStockDividends ();
  };
}

int
main ()
{
  project (test_Session1 (), PROJECT_NAME, PROJECT_NAME, "Session 1");
}
