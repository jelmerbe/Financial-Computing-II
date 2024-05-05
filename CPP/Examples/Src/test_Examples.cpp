#include "Examples/Examples.hpp"
#include "Examples/Output.hpp"
#include "cfl/Data.hpp"
#include "test/Black.hpp"
#include "test/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;
using namespace cfl::Data;

// DATA CURVES FOR FINANCIAL MODELS

void
yield ()
{
  test::print ("CONSTRUCTION OF YIELD CURVE FROM DISCOUNT CURVE");

  double dInitialTime = 2.;

  print (dInitialTime, "initial time", true);

  Function uDiscount = test::Data::getDiscountCurve (dInitialTime);
  Function uYield = prb::yield (uDiscount, dInitialTime);
  double dInterval = 4.75;

  test::Data::print ("input discount factors:", uDiscount,
                     dInitialTime + 0.001, dInterval);

  test::Data::print (uYield, dInitialTime + 0.001, dInterval);
}

void
yieldShape1 ()
{
  test::print ("YIELD SHAPE 1");

  double dLambda = 0.05;
  double dInitialTime = 2.;

  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);
  Function uYield = prb::yieldShape1 (dLambda, dInitialTime);
  double dInterval = 4.75;
  test::Data::print (uYield, dInitialTime + 0.001, dInterval);
}

void
yieldShape2 ()
{
  test::print ("YIELD SHAPE 2");

  double dLambda = 0.05;
  double dInitialTime = 2.;

  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);
  Function uYield = prb::yieldShape2 (dLambda, dInitialTime);
  double dInterval = 4.75;
  test::Data::print (uYield, dInitialTime + 0.001, dInterval);
}

void
forwardFX ()
{
  test::print ("FORWARD PRICES FOR EXCHANGE RATES");

  double dSpotFX = 100;
  double dDom = 0.12;
  double dFor = 0.05;
  double dInitialTime = 1.;

  print (dInitialTime, "initial time");
  print (dSpotFX, "spot FX rate");
  print (dDom, "domestic interest rate");
  print (dFor, "foreign interest rate", true);

  Function uDomestic = cfl::Data::discount (dDom, dInitialTime);
  Function uForeign = cfl::Data::discount (dFor, dInitialTime);
  Function uForwardFX = prb::forwardFX (dSpotFX, uDomestic, uForeign);
  double dInterval = 0.5;
  test::Data::print (uForwardFX, dInitialTime, dInterval);
}

void
forwardCouponBond ()
{
  test::print ("FORWARD PRICES FOR A COUPON BOND");

  cfl::Data::CashFlow uBond = test::swapParameters ();
  uBond.notional = 1.;
  double dRate = uBond.rate;
  double dInitialTime = 1.;
  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  test::print (dRate, "interest rate");
  test::print (dInitialTime, "initial time", true);

  test::printCashFlow (uBond, "bond parameters");

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
      double dRate = uBond.rate;
      double dPeriod = uBond.period;
      double dMaturity = dInitialTime + dPeriod * uBond.numberOfPayments;
      Function uForwardCouponBond = prb::forwardCouponBond (
          dRate, dPeriod, dMaturity, uDiscount, dInitialTime, bClean);
      double dInterval = uBond.period * uBond.numberOfPayments / 1.1;
      test::Data::print (uForwardCouponBond, dInitialTime, dInterval);
    }
}

void
callBlack ()
{
  test::print ("PRICE OF CALL IN BLACK MODEL");

  double dDF = 0.9;
  double dF = 100;
  double dSigma = 0.2;
  double dK = dF * 0.95;
  double dT = 0.75;

  print (dK, "strike");
  print (dT, "time to maturity");
  print (dDF, "discount factor");
  print (dF, "forward price");
  print (dSigma, "volatility", true);

  double dC = prb::callBlack (dK, dT, dDF, dF, dSigma);
  print (dC, "call price", true);
}

void
callVegaBlack ()
{
  test::print ("VEGA OF CALL IN BLACK MODEL");

  double dDF = 0.9;
  double dF = 100;
  double dSigma = 0.2;
  double dK = dF * 0.95;
  double dT = 0.75;

  print (dK, "strike");
  print (dT, "time to maturity");
  print (dDF, "discount factor");
  print (dF, "forward price");
  print (dSigma, "volatility", true);

  double dVega = prb::callVegaBlack (dK, dT, dDF, dF, dSigma);
  print (dVega, "call vega", true);
}

// ROOT FINDING

void
callImplVolBlack ()
{
  test::print ("IMPLIED VOLATILITY OF CALL IN BLACK MODEL");

  double dDF = 0.9;
  double dF = 100;
  double dSigma1 = 0.2;
  double dSigma0 = 0.15;
  double dK = dF * 0.95;
  double dT = 0.75;
  double dErr = cfl::EPS;
  double dC = prb::callBlack (dK, dT, dDF, dF, dSigma1);

  print (dK, "strike");
  print (dT, "time to maturity");
  print (dDF, "discount factor");
  print (dF, "forward price");
  print (dC, "call price");
  print (dSigma1, "true volatility");
  print (dSigma0, "volatility guess");
  print (dErr, "absolute error for the price", true);

  double dSigma = prb::callImplVolBlack (dK, dT, dDF, dF, dC, dSigma0, dErr);
  double dErr1 = abs (prb::callBlack (dK, dT, dDF, dF, dSigma) - dC);
  print (dSigma, "implied volatility");
  print (dErr1, "actual error for price", true);
}

void
callImplVolBlack_rootD ()
{
  test::print ("IMPLIED VOLATILITY OF CALL IN BLACK MODEL WITH cfl::RootD");

  double dDF = 0.9;
  double dF = 100;
  double dSigma1 = 0.2;
  double dSigma0 = 0.15;
  double dK = dF * 0.95;
  double dT = 0.75;
  double dErr = cfl::EPS;
  double dC = prb::callBlack (dK, dT, dDF, dF, dSigma1);

  print (dK, "strike");
  print (dT, "time to maturity");
  print (dDF, "discount factor");
  print (dF, "forward price");
  print (dC, "call price");
  print (dSigma1, "true volatility");
  print (dSigma0, "volatility guess");
  print (dErr, "absolute error for the price", true);
  using namespace cfl::NRootD;

  std::vector<cfl::RootD> uSolver
      = { newton (dErr), secant (dErr), steffenson (dErr) };
  std::vector<std::string> uName = { "Newton", "secant", "Steffenson" };

  for (unsigned i = 0; i < uName.size (); i++)
    {
      string sTitle ("method = ");
      sTitle += uName[i];
      print (sTitle);
      double dSigma
          = prb::callImplVolBlack (dK, dT, dDF, dF, dC, dSigma0, uSolver[i]);
      double dErr1 = abs (prb::callBlack (dK, dT, dDF, dF, dSigma) - dC);
      print (dSigma, "implied volatility");
      print (dErr1, "actual error for price", true);
    }
}

void
callsImplVol ()
{
  test::print ("IMPLIED VOLATILITY OF CALLS IN BLACK MODEL WITH cfl::Root");

  double dRate = 0.07;
  double dSpot = 100;
  double dCostOfCarry = 0.02;
  double dInitialTime = 1.5;
  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  Function uForward = cfl::Data::forward (dSpot, dCostOfCarry, dInitialTime);

  double dSigma0 = 0.01;
  double dSigma1 = 0.5;

  print (dRate, "yield");
  print (dSpot, "spot");
  print (dCostOfCarry, "cost-of-carry");
  print (dInitialTime, "initial time");
  print (dSigma0, "lower bound for sigma");
  print (dSigma1, "upper bound for sigma", true);

  unsigned iCalls = 3;
  std::vector<cfl::Data::Option> uCalls (iCalls);

  double dSigmaRef = 0.5 * (dSigma0 + dSigma1);
  double dValue = 0.;

  for (unsigned i = 0; i < iCalls; i++)
    {
      Option &rCall = uCalls[i];
      double dT = dInitialTime + 0.5 * (1. + i);
      double dK = uForward (dT);
      double dN = std::pow (-2, i);
      double dSigma = dSigmaRef + i * 0.04;
      dValue += prb::callBlack (dK, dT - dInitialTime, uDiscount (dT),
                                uForward (dT), dSigma);
      rCall.maturity = dT;
      rCall.strike = dK;
      rCall.number = dN;

      print (i, "index of call");
      print (dT, "maturity");
      print (dK, "strike");
      print (dN, "the number of calls", true);
    }

  print (dValue, "the value of the portfolio", true);

  double dEpsR = 10E-6;
  double dEpsA = 10E-8;
  test::print (dEpsR, "relative error");
  test::print (dEpsA, "absolute error");
  test::print ("method = brent", true);

  cfl::Root uRoot = cfl::NRoot::brent (dEpsR, dEpsA);

  double dImplSigma
      = prb::callsImplVol (uCalls, dValue, uDiscount, uForward, dInitialTime,
                           dSigma0, dSigma1, uRoot);
  double dImplValue = 0.;

  for (unsigned i = 0; i < iCalls; i++)
    {
      const Option &rCall = uCalls[i];
      double dT = rCall.maturity;
      double dK = rCall.strike;
      double dN = rCall.number;
      dImplValue += dN
                    * prb::callBlack (dK, dT - dInitialTime, uDiscount (dT),
                                      uForward (dT), dImplSigma);
    }

  print (dImplSigma, "the implied volatility");
  print (dImplValue, "the implied value");
  print (std::abs (dValue - dImplValue), "the difference between the values",
         true);
}

// INTERPOLATION OF DATA CURVES

void
discountLogLinInterp ()
{
  test::print ("LOG LINEAR INTERPOLATION OF DISCOUNT CURVE");

  double dInitialTime = 1.;
  auto uDF = test::Data::getDiscount (dInitialTime);

  Function uDiscount
      = prb::discountLogLinInterp (uDF.first, uDF.second, dInitialTime);

  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::print (uDiscount, dInitialTime, dInterval);
}

void
forwardCarryInterp ()
{
  test::print ("FORWARD PRICES BY INTERPOLATION OF COST-OF-CARRY RATES");

  double dSpot = 100;
  double dInitialTime = 1.;
  auto uF = test::Data::getForward (dSpot, dInitialTime);
  double dInitialCarryRate = std::log (uF.second.front () / dSpot)
                             / (uF.first.front () - dInitialTime);
  print (dInitialCarryRate, "initial carry rate", true);
  Interp uInterp = NInterp::cspline ();

  Function uResult = prb::forwardCarryInterp (
      dSpot, uF.first, uF.second, dInitialCarryRate, dInitialTime, uInterp);

  print ("interpolation with cubic spline:");
  double dInterval = uF.first.back () - dInitialTime;
  test::Data::print (uResult, dInitialTime, dInterval);
}

void
forwardCarrySteffenInterp ()
{
  test::print (
      "FORWARD PRICES BY STEFFEN INTERPOLATION OF COST-OF-CARRY RATES");

  double dSpot = 100;
  double dInitialTime = 1.;
  auto uF = test::Data::getForward (dSpot, dInitialTime);

  Function uResult = prb::forwardCarrySteffenInterp (dSpot, uF.first,
                                                     uF.second, dInitialTime);

  double dInterval = uF.first.back () - dInitialTime;
  test::Data::print (uResult, dInitialTime, dInterval);
}

// LEAST-SQUARES FITTING OF DATA CURVES

const std::string c_sDF ("Fitted discount factors and their errors:");
const std::string c_sConstYield ("We fit with constant yield.");

void
discountYieldFit ()
{
  test::print ("DISCOUNT CURVE OBTAINED BY LEAST-SQUARES FIT OF YIELD CURVE");

  double dInitialTime = 1.;

  auto uDF = test::Data::getDiscount (dInitialTime);

  print (c_sConstYield);
  cfl::Fit uFit = cfl::NFit::linear (cfl::Function (1.));

  Function uErr;
  Function uDiscount = prb::discountYieldFit (uDF.first, uDF.second,
                                              dInitialTime, uFit, uErr);

  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
discountConstYieldFit ()
{
  test::print ("LEAST-SQUARES FIT OF DISCOUNT CURVE WITH CONSTANT YIELD");

  double dInitialTime = 1.;
  auto uDF = test::Data::getDiscount (dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uDiscount = prb::discountConstYieldFit (uDF.first, uDF.second,
                                                   dInitialTime, uErr, uParam);
  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
discountNelsonSiegelFit ()
{
  test::print ("LEAST-SQUARES FIT OF DISCOUNT CURVE IN NELSON-SIEGEL MODEL");

  double dLambda = 0.05;
  double dInitialTime = 1.;

  print (dLambda, "lambda");
  auto uDF = test::Data::getDiscount (dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uDiscount = prb::discountNelsonSiegelFit (
      uDF.first, uDF.second, dLambda, dInitialTime, uErr, uParam);
  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
discountVasicekFit ()
{
  test::print ("LEAST-SQUARES FIT OF DISCOUNT CURVE IN VASICEK MODEL");

  double dLambda = 0.05;
  double dSigma = 0.01;
  double dInitialTime = 1.;

  print (dLambda, "lambda");
  print (dSigma, "sigma");
  auto uDF = test::Data::getDiscount (dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uDiscount = prb::discountVasicekFit (
      uDF.first, uDF.second, dLambda, dSigma, dInitialTime, uErr, uParam);
  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

// OPTIONS ON A SINGLE STOCK IN BLACK MODEL

MultiFunction
put (AssetModel &rModel)
{
  test::print ("EUROPEAN PUT OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dMaturity = test::c_dMaturity;

  print (dStrike, "strike");
  print (dMaturity, "maturity", true);

  return prb::put (dStrike, dMaturity, rModel);
}

MultiFunction
americanPut (AssetModel &rModel)
{
  test::print ("AMERICAN PUT OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();

  print (dStrike, "strike", true);
  test::print (uExerciseTimes.begin (), uExerciseTimes.end (),
               "exercise times");

  return prb::americanPut (dStrike, uExerciseTimes, rModel);
}

MultiFunction
barrierUpDownOut (AssetModel &rModel)
{
  test::print ("BARRIER UP-OR-DOWN-AND-OUT OPTION IN ASSET MODEL");

  double dLowerBarrier = test::c_dSpot * 0.9;
  double dUpperBarrier = test::c_dSpot * 1.1;
  double dNotional = test::c_dNotional;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();

  print (dLowerBarrier, "lower barrier");
  print (dUpperBarrier, "upper barrier");
  print (dNotional, "notional", true);
  test::print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::barrierUpDownOut (dNotional, dLowerBarrier, dUpperBarrier,
                                uBarrierTimes, rModel);
}

cfl::MultiFunction
forwardOnAverageSpot (cfl::AssetModel &rModel)
{
  print ("FORWARD ON AVERAGE SPOT IN ASSET MODEL");

  const std::vector<double> uAverTimes = test::barrierTimes ();
  print (uAverTimes.begin (), uAverTimes.end (), "averaging times");

  return prb::forwardOnAverageSpot (uAverTimes, rModel);
}

cfl::MultiFunction
forwardOnAverageSpotDetermRates (cfl::AssetModel &rModel)
{
  print ("FORWARD ON AVERAGE SPOT IN ASSET MODEL WITH DETERMINISTIC INTEREST "
         "RATES");

  const std::vector<double> uAverTimes = test::barrierTimes ();
  print (uAverTimes.begin (), uAverTimes.end (), "averaging times");

  return prb::forwardOnAverageSpotDetermRates (uAverTimes, rModel);
}

MultiFunction
downOutAmericanCall (AssetModel &rModel)
{
  test::print ("DOWN-AND-OUT AMERICAN CALL OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();
  double dLowerBarrier = test::c_dSpot * 0.9;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();

  print (dStrike, "strike");
  print (dLowerBarrier, "lower barrier", true);
  test::print (uExerciseTimes.begin (), uExerciseTimes.end (),
               "exercise times");
  test::print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::downOutAmericanCall (dLowerBarrier, uBarrierTimes, dStrike,
                                   uExerciseTimes, rModel);
}

MultiFunction
swing (AssetModel &rModel)
{
  test::print ("SWING OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();
  unsigned iNumberOfExercises = uExerciseTimes.size () / 3;

  print (dStrike, "strike");
  print (iNumberOfExercises, "maximal number of exercises", true);
  test::print (uExerciseTimes.begin (), uExerciseTimes.end (),
               "exercise times");

  return prb::swing (dStrike, uExerciseTimes, iNumberOfExercises, rModel);
}

MultiFunction
fxCrossCurrencyCap (AssetModel &rModel)
{
  test::print ("FX CROSS-CURRENCY CAP IN ASSET MODEL");

  cfl::Data::CashFlow uCap = test::swapParameters ();
  uCap.rate = test::c_dYield - test::c_dDividendYield;
  test::printCashFlow (uCap, "cap parameters");

  return prb::fxCrossCurrencyCap (uCap, rModel);
}

// INTEREST RATE OPTIONS IN HULL-WHITE MODEL

cfl::MultiFunction
cap (InterestRateModel &rModel)
{
  test::print ("CAP IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;

  test::printCashFlow (uCapParameters, "cap parameters");

  return prb::cap (uCapParameters, rModel);
}

cfl::MultiFunction
swap (InterestRateModel &rModel, bool bPayFloat)
{
  test::print ("SWAP IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;

  test::printSwap (uSwap, "swap parameters");

  return prb::swap (uSwap, rModel);
}

cfl::MultiFunction
swaption (InterestRateModel &rModel, bool bPayFloat)
{
  test::print ("SWAPTION IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;
  double dMaturity = test::c_dMaturity;

  test::printSwap (uSwap, "swap parameters");
  print (dMaturity, "maturity", true);

  return prb::swaption (uSwap, dMaturity, rModel);
}

cfl::MultiFunction
cancellableCollar (InterestRateModel &rModel)
{
  test::print ("CANCELLABLE COLLAR IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;
  double dFloorRate = test::c_dYield * 0.9;

  test::printCashFlow (uCapParameters, "cap parameters");
  print (dFloorRate, "floor rate", true);

  return prb::cancellableCollar (uCapParameters, dFloorRate, rModel);
}

cfl::MultiFunction
downOutCap (InterestRateModel &rModel)
{
  test::print ("DOWN-AND-OUT CAP IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;
  double dLowerBarrier = test::c_dYield * 0.9;

  test::printCashFlow (uCapParameters, "cap parameters");
  print (dLowerBarrier, "lower barrier", true);

  return prb::downOutCap (uCapParameters, dLowerBarrier, rModel);
}

cfl::MultiFunction
futuresOnRate (InterestRateModel &rModel)
{
  print ("RATE FUTURES IN INTEREST RATE MODEL");

  double dRatePeriod = test::c_dPeriod;
  unsigned iFuturesTimes = 20;
  double dMaturity = 0.25;

  print (dRatePeriod, "period for float rate");
  print (iFuturesTimes, "number of futures times");
  print (dMaturity, "maturity", true);

  return prb::futuresOnRate (dRatePeriod, iFuturesTimes, dMaturity, rModel);
}

cfl::MultiFunction
dropLockSwap (InterestRateModel &rModel, bool bPayFloat)
{
  test::print ("DROP-LOCK SWAP IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;

  double dLowerRate = uSwap.rate - 0.015;
  double dUpperRate = uSwap.rate + 0.025;

  test::printSwap (uSwap, "swap parameters");
  print (dLowerRate, "lower rate");
  print (dUpperRate, "upper rate", true);

  return prb::dropLockSwap (uSwap, dLowerRate, dUpperRate, rModel);
}

cfl::MultiFunction
autoCap (InterestRateModel &rModel)
{
  test::print ("AUTO CAP IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCap = test::swapParameters ();
  unsigned iNumberOfCaplets = uCap.numberOfPayments / 2;

  test::printCashFlow (uCap, "cap parameters");
  print (iNumberOfCaplets, "number of paid caplets", true);

  return prb::autoCap (uCap, iNumberOfCaplets, rModel);
}

std::function<void ()>
test_Examples ()
{
  return [] () {
    print ("DATA CURVES FOR FINANCIAL MODELS");

    yield ();
    yieldShape1 ();
    yieldShape2 ();
    forwardFX ();
    forwardCouponBond ();
    callBlack ();
    callVegaBlack ();

    print ("ROOT FINDING");

    callImplVolBlack ();
    callImplVolBlack_rootD ();
    callsImplVol ();

    print ("INTERPOLATION OF DATA CURVES");

    discountLogLinInterp ();
    forwardCarryInterp ();
    forwardCarrySteffenInterp ();

    print ("LEAST-SQUARES FITTING OF DATA CURVES");

    discountYieldFit ();
    discountConstYieldFit ();
    discountNelsonSiegelFit ();
    discountVasicekFit ();

    print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uBlack = test::Black::model ();
    test::report (put, uBlack);
    test::report (americanPut, uBlack);
    test::report (barrierUpDownOut, uBlack);
    test::report (forwardOnAverageSpot, uBlack);
    test::report (forwardOnAverageSpotDetermRates, uBlack);
    test::report (downOutAmericanCall, uBlack);
    test::report (swing, uBlack);
    test::report (fxCrossCurrencyCap, uBlack);

    print ("INTEREST RATE OPTIONS IN HULL-WHITE MODEL");

    InterestRateModel uHullWhite = test::HullWhite::model ();
    test::report (cap, uHullWhite);
    test::report (swap, uHullWhite);
    test::report (swaption, uHullWhite);
    test::report (cancellableCollar, uHullWhite);
    test::report (downOutCap, uHullWhite);
    test::report (futuresOnRate, uHullWhite);
    test::report (dropLockSwap, uHullWhite);
    test::report (autoCap, uHullWhite);
  };
}

int
main ()
{
  project (test_Examples (), PROJECT_NAME, PROJECT_NAME, "Examples");
}
