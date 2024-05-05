#include "cfl/Data.hpp"
#include "cfl/Fit.hpp"
#include "test/Data.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;

void
test::Data::print (const Function &rData, double dStartTime, double dInterval,
                   unsigned iPoints)
{
  test::Data::print ("VALUES VERSUS TIME:", rData, dStartTime, dInterval,
                     iPoints);
}

void
test::Data::print (const std::function<double (double)> &rData,
                   double dStartTime, double dInterval, unsigned iPoints)
{
  test::Data::print (Function (rData), dStartTime, dInterval, iPoints);
}

void
test::Data::print (const std::string &rTitle, const cfl::Function &rData,
                   double dStartTime, double dInterval, unsigned iPoints)
{
  std::valarray<double> uArg
      = getArg (dStartTime, dStartTime + dInterval, iPoints);

  std::vector<Function> uValues (1, rData);
  std::vector<std::string> uNames (1, "value");
  unsigned iColumn = 10;
  unsigned iTime = 8;
  unsigned iSpace = 6;

  printTable (uValues, uNames, uArg, rTitle, iColumn, iTime, iSpace, "time");
}

void
test::Data::print (const std::string &rTitle, const cfl::Function &rData,
                   const cfl::Function &rErr, double dStartTime,
                   double dInterval, unsigned iPoints)
{
  PRECONDITION (dInterval > cfl::EPS);

  std::valarray<double> uArg
      = getArg (dStartTime, dStartTime + dInterval, iPoints);

  std::vector<Function> uValues = { rData, rErr };
  std::vector<std::string> uNames = { "value", "err" };
  unsigned iColumn = 10;
  unsigned iTime = 8;
  unsigned iSpace = 6;

  printTable (uValues, uNames, uArg, rTitle, iColumn, iTime, iSpace, "time");
}

void
test::Data::print (const std::string &sTitle,
                   const std::vector<double> &rTimes,
                   const std::vector<double> &rValues,
                   const std::string &sTimes, const std::string &sValues)
{
  std::vector<std::vector<double> > uValues = { rTimes, rValues };
  std::vector<std::string> uNames = { sTimes, sValues };
  unsigned iColumn = 10;
  unsigned iSpace = 6;
  unsigned iMaxRows = 60;

  printTable (uValues, uNames, sTitle, iColumn, iSpace, iMaxRows);
}

void
test::Data::printFit (const cfl::FitParam &rParam)
{
  const std::valarray<double> &rFit = rParam.fit;
  const std::valarray<double> &rCov = rParam.cov;
  test::print ("Fitted coefficients and their covariance matrix:");

  std::function<double (double)> uRound = roundResult ();

  unsigned iColumn = 12;
  unsigned iSpace = 6;
  unsigned iTitleSpace = (iSpace)*rFit.size ();
  cout << std::setw (iColumn) << "value" << setw (iTitleSpace) << ""
       << setw (iColumn) << "covariance matrix" << endl;

  for (unsigned i = 0; i < rFit.size (); i++)
    {
      cout << std::setw (iColumn) << uRound (rFit[i]) << std::setw (iSpace);
      for (unsigned j = 0; j < rFit.size (); j++)
        {
          cout << "" << std::setw (iColumn)
               << uRound (rCov[i * rFit.size () + j]) << std::setw (iSpace);
        }
      cout << endl;
    }
  cout << endl;

  test::print (rParam.chi2, "chi^2 error", true);
}

const std::string c_sInDF ("Input discount factors:");
const std::string c_sInForward ("Input forward prices:");

namespace testData
{
std::pair<std::vector<double>, std::vector<double> >
getDiscount (double dInitialTime, double dRate = 0.07, double dLambda = 0.22,
             bool bPrint = true)
{
  unsigned iTimes = 12;
  std::vector<double> uTimes (iTimes);
  double dPeriod = 0.5;
  uTimes.front () = dInitialTime + dPeriod;
  std::transform (uTimes.begin (), uTimes.end () - 1, uTimes.begin () + 1,
                  [dPeriod] (double dX) { return dX + dPeriod; });
  std::vector<double> uDiscountFactors (iTimes);
  Function uYield (
      [dRate, dLambda, dInitialTime] (double dT) {
        double dX = std::max (dLambda * (dT - dInitialTime), cfl::EPS);
        return dRate * (1. - std::exp (-dX)) / dX;
      },
      dInitialTime);

  std::transform (uTimes.begin (), uTimes.end (), uDiscountFactors.begin (),
                  cfl::Data::discount (uYield, dInitialTime));

  if (bPrint)
    {
      test::print (dInitialTime, "initial time", true);
      test::Data::print (c_sInDF, uTimes, uDiscountFactors);
    };

  return { uTimes, uDiscountFactors };
}

std::pair<std::vector<double>, std::vector<double> >
getForward (double dSpot, double dInitialTime, double dLambda = 0.22)
{
  unsigned iTimes = 10;
  std::vector<double> uTimes (iTimes);
  double dPeriod = 0.5;
  uTimes.front () = dInitialTime + dPeriod;
  std::transform (uTimes.begin (), uTimes.end () - 1, uTimes.begin () + 1,
                  [dPeriod] (double dX) { return dX + dPeriod; });
  std::vector<double> uForward (iTimes);
  double dRate = 0.07;
  Function uCarry (
      [dRate, dLambda, dInitialTime] (double dT) {
        double dX = std::max (dLambda * (dT - dInitialTime), cfl::EPS);
        return dRate * (1. - std::exp (-dX)) / dX;
      },
      dInitialTime);

  std::transform (uTimes.begin (), uTimes.end (), uForward.begin (),
                  cfl::Data::forward (dSpot, uCarry, dInitialTime));

  test::print (dSpot, "spot");
  test::print (dInitialTime, "initial time", true);
  test::Data::print (c_sInForward, uTimes, uForward);

  return { uTimes, uForward };
}

std::tuple<std::vector<double>, std::vector<double>, std::vector<double> >
getDiscountFX (double dSpot, double dInitialTime)
{
  double dDom = 0.044;
  double dLambdaDom = 0.14;
  double dFor = 0.1;
  double dLambdaFor = 0.07;
  auto uD = getDiscount (dInitialTime, dDom, dLambdaDom, false);
  auto uF = getDiscount (dInitialTime, dFor, dLambdaFor, false);

  test::print (dSpot, "spot FX rate");
  test::print (dInitialTime, "initial time", true);

  std::vector<std::vector<double> > uValues
      = { uD.first, uD.second, uF.second };
  std::vector<std::string> uNames = { "time", "domestic DF", "foreign DF" };
  unsigned iColumn = 10;
  unsigned iSpace = 6;
  unsigned iMaxRows = 20;
  test::printTable (uValues, uNames, c_sInDF, iColumn, iSpace, iMaxRows);

  return { uD.first, uD.second, uF.second };
}

std::pair<std::vector<double>, std::vector<double> >
getRate (double dInitialTime)
{
  unsigned iTimes = 6;
  std::vector<double> uRatePeriods (iTimes);
  std::vector<double> uRates (iTimes);
  double dRatePeriodStep = 1. / (1. + iTimes);
  double dShortRate = 0.04;
  double dLongRate = dShortRate + 0.02;
  double dFloatRateStep = (dLongRate - dShortRate) / (1. + iTimes);
  uRatePeriods.front () = dRatePeriodStep;
  std::transform (uRatePeriods.begin (), uRatePeriods.end () - 1,
                  uRatePeriods.begin () + 1, [dRatePeriodStep] (double dX) {
                    return dX + dRatePeriodStep;
                  });
  uRates.front () = dShortRate;
  std::transform (
      uRates.begin (), uRates.end () - 1, uRates.begin () + 1,
      [dFloatRateStep] (double dX) { return dX + dFloatRateStep; });
  test::print (dInitialTime, "initial time", true);
  test::Data::print ("interest rates", uRatePeriods, uRates, "periods",
                     "rates");

  return { uRatePeriods, uRates };
}
} // namespace testData

std::pair<std::vector<double>, std::vector<double> >
test::Data::getDiscount (double dInitialTime)
{
  return testData::getDiscount (dInitialTime);
}

cfl::Function
test::Data::getDiscountCurve (double dInitialTime)
{
  return Function (
      [dInitialTime] (double dT) {
        ASSERT (dT >= dInitialTime);
        double dY = 0.07;
        double dFactor = 0.2;
        double dYield = dY / (1. + dFactor * (dT - dInitialTime));
        double dDF = exp (-dYield * (dT - dInitialTime));
        return dDF;
      },
      dInitialTime);
}

std::pair<std::vector<double>, std::vector<double> >
test::Data::getForward (double dSpot, double dInitialTime)
{
  return testData::getForward (dSpot, dInitialTime);
}

std::tuple<std::vector<double>, std::vector<double>, std::vector<double> >
test::Data::getDiscountFX (double dSpot, double dInitialTime)
{
  return testData::getDiscountFX (dSpot, dInitialTime);
}

std::pair<std::vector<double>, std::vector<double> >
test::Data::getRate (double dInitialTime)
{
  return testData::getRate (dInitialTime);
}

std::vector<double>
test::Data::getSwapRates (double dPeriod, double dInitialTime)
{
  double dRate = 0.12;
  double dRateStep = 0.005;

  unsigned iSwaps = 10;
  std::vector<double> uSwapRates (iSwaps);

  uSwapRates.front () = dRate;
  std::transform (uSwapRates.begin (), uSwapRates.end () - 1,
                  uSwapRates.begin () + 1,
                  [dRateStep] (double dX) { return dX + dRateStep; });
  test::print (dPeriod, "period between payments");
  test::print (dInitialTime, "initial time", true);
  test::print (uSwapRates.begin (), uSwapRates.end (), "swap rates");
  return uSwapRates;
}

std::pair<std::vector<double>, std::vector<unsigned> >
test::Data::getSwapRatesPeriods (double dPeriod, double dInitialTime)
{
  double dRate = 0.12;
  double dRateStep = 0.005;

  unsigned iSwaps = 10;

  std::vector<unsigned> uSwapPeriods (iSwaps, 1);
  unsigned iStep = 3;
  std::transform (uSwapPeriods.begin (), uSwapPeriods.end () - 1,
                  uSwapPeriods.begin () + 1,
                  [iStep] (unsigned iN) { return iN + iStep; });
  std::vector<double> uSwapRates (iSwaps);

  uSwapRates.front () = dRate;
  std::transform (uSwapRates.begin (), uSwapRates.end () - 1,
                  uSwapRates.begin () + 1,
                  [dRateStep] (double dX) { return dX + dRateStep; });

  test::print (dInitialTime, "initial time");
  test::print (dPeriod, "swap period", true);
  std::vector<double> uPeriods (iSwaps);
  std::copy (uSwapPeriods.begin (), uSwapPeriods.end (), uPeriods.begin ());
  test::Data::print ("swap rates and number of periods:", uSwapRates, uPeriods,
                     "swap rate", "number of periods");
  return { uSwapRates, uSwapPeriods };
}

const std::string c_sInVol ("Input volatilities:");

std::pair<std::vector<double>, std::vector<double> >
test::Data::getVol (double dInitialTime)
{
  double dSigma = 0.035;
  double dLambda = 0.25;
  unsigned iTimes = 10;
  std::vector<double> uTimes (iTimes);
  double dPeriod = 0.5;
  uTimes.front () = dInitialTime + dPeriod;
  std::transform (uTimes.begin (), uTimes.end () - 1, uTimes.begin () + 1,
                  [dPeriod] (double dX) { return dX + dPeriod; });
  std::vector<double> uVols (iTimes);
  std::transform (uTimes.begin (), uTimes.end (), uVols.begin (),
                  cfl::Data::volatility (dSigma, dLambda, dInitialTime));

  test::print (dInitialTime, "initial time", true);
  test::Data::print (c_sInVol, uTimes, uVols, "time", "volatility");

  return { uTimes, uVols };
}

std::tuple<std::vector<double>, std::vector<double>, std::vector<double> >
test::Data::getVolBonds (double dInitialTime)
{
  double dSigma = 0.035;
  unsigned iSize = 12;
  double dPeriod = 0.25;
  double dLambda = 0.25;
  std::vector<double> uOptionTimes (iSize);
  std::vector<double> uBondTimes (iSize);
  std::vector<double> uVols (iSize);

  cfl::Function uVol = cfl::Data::volatility (dSigma, dLambda, dInitialTime);

  std::function<double (double)> uBondShape
      = [dLambda, dInitialTime] (double dTime) {
          PRECONDITION (dTime >= dInitialTime);
          double dT = dTime - dInitialTime;
          return (std::abs (dLambda) <= cfl::EPS)
                     ? dT
                     : (1. - std::exp (-dLambda * dT)) / dLambda;
        };
  cfl::Function uShape (uBondShape, dInitialTime);

  for (unsigned int i = 0; i < iSize; i++)
    {
      uOptionTimes[i] = dInitialTime + (i + 1) * dPeriod;
      uBondTimes[i] = uOptionTimes[i] + dPeriod / 2.;
      uVols[i] = (uShape (uBondTimes[i]) - uShape (uOptionTimes[i]))
                 * uVol (uOptionTimes[i]);
      ASSERT (uVols[i] > 0);
    }

  test::print (dInitialTime, "initial time", true);

  std::vector<std::vector<double> > uValues
      = { uOptionTimes, uBondTimes, uVols };
  std::vector<std::string> uNames
      = { "option maturity", "DF maturity", "volatility" };
  unsigned iColumn = 14;
  unsigned iSpace = 6;
  unsigned iMaxRows = 60;
  printTable (uValues, uNames, c_sInVol, iColumn, iSpace, iMaxRows);

  return { uOptionTimes, uBondTimes, uVols };
}

std::pair<std::vector<double>, std::vector<double> >
test::Data::getFXSwapRates (double dSpotFX, double dPeriod,
                            double dInitialTime)
{
  double dDRate = 0.12;
  double dDRateStep = 0.005;
  double dFRate = 0.07;
  double dFRateStep = -0.005;
  unsigned iSwaps = 10;
  std::vector<double> uDSwapRates (iSwaps);
  std::vector<double> uFSwapRates (iSwaps);
  uDSwapRates.front () = dDRate;
  std::transform (uDSwapRates.begin (), uDSwapRates.end () - 1,
                  uDSwapRates.begin () + 1,
                  [dDRateStep] (double dX) { return dX + dDRateStep; });
  uFSwapRates.front () = dFRate;
  std::transform (uFSwapRates.begin (), uFSwapRates.end () - 1,
                  uFSwapRates.begin () + 1,
                  [dFRateStep] (double dX) { return dX + dFRateStep; });

  test::print (dSpotFX, "spot fx rate");
  test::print (dPeriod, "period between payments");
  test::print (dInitialTime, "initial time", true);

  std::vector<vector<double> > uValues = { uDSwapRates, uFSwapRates };
  std::vector<string> uNames = { "domestic", "foreign" };
  test::printTable (uValues, uNames, "fixed swap rates:");

  return { uDSwapRates, uFSwapRates };
}
