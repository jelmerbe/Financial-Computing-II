#include "Session2/Output.hpp"
#include "Session2/Session2.hpp"
#include "test/Black.hpp"
#include "test/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

// ROOT FINDING

void
couponBond ()
{
  test::print ("PRICE FOR A COUPON BOND");

  cfl::Data::CashFlow uBond = test::swapParameters ();
  uBond.notional = 1.;
  double dYTM = uBond.rate * 1.1;
  double dIssueTime = 1.;
  double dInitialTime = dIssueTime + 0.5 * uBond.period;
  double dMaturity = dIssueTime + uBond.period * uBond.numberOfPayments;
  Function uDiscount = cfl::Data::discount (dYTM, dInitialTime);
  test::print (dYTM, "yield to maturity", true);

  test::printCashFlow (uBond, "bond parameters");

  test::print (dIssueTime, "issue time");
  test::print (dInitialTime, "initial time");
  test::print (dMaturity, "maturity", true);

  bool bClean = false;
  double dPD = prb::couponBond (uBond.rate, uBond.period, dMaturity, dYTM,
                                dInitialTime, bClean);
  test::print (dPD, "dirty price");

  bClean = true;
  double dPC = prb::couponBond (uBond.rate, uBond.period, dMaturity, dYTM,
                                dInitialTime, bClean);
  test::print (dPC, "clean price");

  print (dPD - dPC, "accrued interest", true);
}

void
durationCouponBond ()
{
  test::print ("DURATION FOR A COUPON BOND");

  cfl::Data::CashFlow uBond = test::swapParameters ();
  uBond.notional = 1.;
  double dYTM = uBond.rate * 1.1;
  double dIssueTime = 1.;
  double dInitialTime = dIssueTime + 0.5 * uBond.period;
  double dMaturity = dIssueTime + uBond.period * uBond.numberOfPayments;
  Function uDiscount = cfl::Data::discount (dYTM, dInitialTime);
  test::print (dYTM, "yield to maturity", true);

  test::printCashFlow (uBond, "bond parameters");

  test::print (dIssueTime, "issue time");
  test::print (dInitialTime, "initial time");
  test::print (dMaturity, "maturity", true);

  double dD = prb::durationCouponBond (uBond.rate, uBond.period, dMaturity,
                                       dYTM, dInitialTime);
  test::print (dD, "dollar duration", true);
}

void
yieldTMCouponBond_simple ()
{
  test::print ("YIELD TO MATURITY FOR COUPON BOND");

  cfl::Data::CashFlow uBond = test::swapParameters ();
  uBond.notional = 1.;
  double dIssueTime = 1.;
  double dInitialTime = dIssueTime + 0.5 * uBond.period;
  double dMaturity = dIssueTime + uBond.period * uBond.numberOfPayments;
  double dPrice = 1. + (dInitialTime - dIssueTime) * uBond.rate;
  double dY0 = uBond.rate / 2;
  double dErr = cfl::EPS;

  test::printCashFlow (uBond, "bond parameters");

  print (dMaturity, "maturity");
  test::print (dPrice, "price");
  test::print (dInitialTime, "initial time");
  test::print (dY0, "initial guess for YTM");
  print (dErr, "expected yield error", true);

  double dYTM = prb::yieldTMCouponBond (uBond.rate, uBond.period, dMaturity,
                                        dInitialTime, dPrice, dY0, dErr);
  test::print (dYTM, "YTM", true);
}

void
yieldTMCouponBond ()
{
  test::print ("YIELD TO MATURITY FOR COUPON BOND WITH cfl::RootD");

  cfl::Data::CashFlow uBond = test::swapParameters ();
  uBond.notional = 1.;
  double dIssueTime = 1.;
  double dInitialTime = dIssueTime + 0.5 * uBond.period;
  double dMaturity = dIssueTime + uBond.period * uBond.numberOfPayments;
  double dPrice = 1. + (dInitialTime - dIssueTime) * uBond.rate;
  double dY0 = uBond.rate / 2;
  double dErrR = 10E-6;
  double dErrA = 10E-8;

  test::printCashFlow (uBond, "bond parameters");

  print (dMaturity, "maturity");
  test::print (dPrice, "price");
  test::print (dInitialTime, "initial time");
  test::print (dY0, "initial guess for YTM");
  print (dErrR, "relative yield error");
  print (dErrA, "absolute yield error");
  print ("method = secant");

  RootD uRoot = cfl::NRootD::secant (dErrR, dErrA);

  double dYTM = prb::yieldTMCouponBond (uBond.rate, uBond.period, dMaturity,
                                        dInitialTime, dPrice, dY0, uRoot);
  test::print (dYTM, "YTM", true);
}

// INTERPOLATION OF DATA CURVES

void
discountYieldInterp ()
{
  test::print ("DISCOUNT CURVE BY INTERPOLATION OF YIELDS");

  double dInitialTime = 1.;

  auto uDF = test::Data::getDiscount (dInitialTime);
  double dR
      = (1. / uDF.second.front () - 1.) / (uDF.first.front () - dInitialTime);
  test::print (dR, "initial short-term rate", true);
  Interp uInterp = NInterp::linear ();
  print ("We use linear interpolation for yields");

  Function uDiscount = prb::discountYieldInterp (uDF.first, uDF.second, dR,
                                                 dInitialTime, uInterp);

  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::print (uDiscount, dInitialTime, dInterval);
}

void
discountYieldSteffenInterp ()
{
  test::print ("DISCOUNT CURVE BY STEFFEN INTERPOLATION OF YIELDS");

  double dInitialTime = 1.;

  auto uDF = test::Data::getDiscount (dInitialTime);
  double dR
      = (1. / uDF.second.front () - 1.) / (uDF.first.front () - dInitialTime);
  test::print (dR, "initial short-term rate", true);

  Function uDiscount = prb::discountYieldSteffenInterp (uDF.first, uDF.second,
                                                        dR, dInitialTime);

  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::print (uDiscount, dInitialTime, dInterval);
}

std::function<void ()>
test_Session2 ()
{
  return [] () {
    print ("ROOT FINDING");

    couponBond ();
    durationCouponBond ();
    yieldTMCouponBond_simple ();
    yieldTMCouponBond ();

    print ("INTERPOLATION OF DATA CURVES");

    discountYieldInterp ();
    discountYieldSteffenInterp ();
  };
}

int
main ()
{
  project (test_Session2 (), PROJECT_NAME, PROJECT_NAME, "Session 2");
}
