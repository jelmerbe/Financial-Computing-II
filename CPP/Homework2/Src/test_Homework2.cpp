#include "Homework2/Homework2.hpp"
#include "Homework2/Output.hpp"
#include "test/Data.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

// ROOT FINDING

void
yieldTMCashFlow ()
{
  test::print ("YIELD TO MATURITY FOR A CASH FLOW");

  double dInitialTime = 1.;
  std::vector<double> uPayments = { 1, -2, 3, 5, 6, -2 };
  std::vector<double> uTimes
      = getTimes (dInitialTime + 0.5, dInitialTime + 3., uPayments.size ());

  double dY = 0.05;
  double dPrice = 0.;
  dPrice = std::inner_product (
      uPayments.begin (), uPayments.end (), uTimes.begin (), dPrice,
      std::plus<double> (),
      [dInitialTime, dY] (double dPayment, double dTime) {
        return dPayment * std::exp (-dY * (dTime - dInitialTime));
      });

  test::print (dPrice, "initial price");
  test::print (dInitialTime, "initial time", true);
  test::Data::print ("cash flow:", uTimes, uPayments);
  double dY0 = 0.0;
  double dY1 = 0.2;
  test::print (dY0, "lower bound");
  test::print (dY1, "upper bound");
  double dEpsR = 10E-6;
  double dEpsA = 10E-8;
  test::print (dEpsR, "relative error for YTM");
  test::print (dEpsA, "absolute error for YTM");
  test::print ("method = brent");

  cfl::Root uRoot = cfl::NRoot::brent (dEpsR, dEpsA);

  double dYTM = prb::yieldTMCashFlow (uPayments, uTimes, dPrice, dInitialTime,
                                      dY0, dY1, uRoot);
  test::print (dYTM, "YTM");
  test::print (std::abs (dYTM - dY), "error", true);
}

void
forwardYTMCashFlow ()
{
  test::print ("FORWARD YIELD TO MATURITY FOR A CASH FLOW");

  double dInitialTime = 1.;
  std::vector<double> uPayments = { 1, -2, -3, 5, 6, 2 };
  double dYield = 0.05;

  cfl::Function uYield (
      [dYield] (double dT) { return dYield * (1. + 0.1 * dT); }, dInitialTime);
  cfl::Function uDiscount = cfl::Data::discount (uYield, dInitialTime);

  std::vector<double> uTimes
      = getTimes (dInitialTime + 0.5, dInitialTime + 3., uPayments.size ());

  std::function<double (double)> uF = [&uDiscount, &uPayments,
                                       &uTimes] (double dT) {
    unsigned iTime = std::lower_bound (uTimes.begin (), uTimes.end (), dT)
                     - uTimes.begin ();
    double dPrice = 0.;
    dPrice = std::inner_product (uPayments.begin () + iTime, uPayments.end (),
                                 uTimes.begin () + iTime, dPrice,
                                 std::plus<double> (),
                                 [&uDiscount] (double dPayment, double dTime) {
                                   return dPayment * uDiscount (dTime);
                                 });
    dPrice /= uDiscount (dT);

    return dPrice;
  };

  cfl::Function uForward (uF, dInitialTime);

  test::print (dInitialTime, "initial time", true);
  test::Data::print ("cash flow:", uTimes, uPayments);
  double dY0 = 0;
  double dY1 = 0.2;
  test::print (dY0, "lower bound");
  test::print (dY1, "upper bound");
  double dEpsR = 10E-6;
  double dEpsA = 10E-8;
  test::print (dEpsR, "relative error for YTM");
  test::print (dEpsA, "absolute error for YTM");
  test::print ("method = brent");

  cfl::Root uRoot = cfl::NRoot::brent (dEpsR, dEpsA);

  Function uForwardYTMCashFlow = prb::forwardYTMCashFlow (
      uPayments, uTimes, uForward, dInitialTime, dY0, dY1, uRoot);
  double dInterval = (uTimes.back () - dInitialTime) / 1.01;
  test::Data::print (uForwardYTMCashFlow, dInitialTime, dInterval);
}

// INTERPOLATION OF DATA CURVES

void
volatilityVarInterp ()
{
  test::print ("VOLATILITY CURVE BY INTERPOLATION OF VARIANCE CURVE");

  double dInitialTime = 1.;
  auto uV = test::Data::getVol (dInitialTime);
  Interp uInterp = NInterp::steffen ();

  Function uResult
      = prb::volatilityVarInterp (uV.first, uV.second, dInitialTime, uInterp);

  print ("We use Steffen interpolation");
  double dInterval = uV.first.back () - dInitialTime;
  test::Data::print (uResult, dInitialTime, dInterval);
}

void
volatilityVarSteffenInterp ()
{
  test::print ("VOLATILITY CURVE BY STEFFEN INTERPOLATION OF VARIANCE CURVE");

  double dInitialTime = 1.;
  auto uV = test::Data::getVol (dInitialTime);

  Function uResult
      = prb::volatilityVarSteffenInterp (uV.first, uV.second, dInitialTime);

  double dInterval = uV.first.back () - dInitialTime;
  test::Data::print (uResult, dInitialTime, dInterval);
}

std::function<void ()>
test_Homework2 ()
{
  return [] () {
    print ("ROOT FINDING");

    yieldTMCashFlow ();
    forwardYTMCashFlow ();

    print ("INTERPOLATION OF DATA CURVES");

    volatilityVarInterp ();
    volatilityVarSteffenInterp ();
  };
}

int
main ()
{
  project (test_Homework2 (), PROJECT_NAME, PROJECT_NAME, "Homework 2");
}
