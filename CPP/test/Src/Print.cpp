#include "test/Print.hpp"
#include "cfl/Macros.hpp"
#include "test/Output.hpp"
#include <random>

using namespace std;
using namespace cfl;

// accessor functions

std::valarray<double>
test::getArg (double dL, double dR, unsigned iN)
{
  PRECONDITION (iN > 0);

  std::valarray<double> uResult (iN);
  double dH = (dR - dL) / (iN - 1);
  double dX = dL;
  for (unsigned iI = 0; iI < iN; iI++)
    {
      uResult[iI] = dX;
      dX += dH;
    }
  uResult[uResult.size () - 1] = dR;
  return uResult;
}

std::vector<double>
test::getTimes (double dInitialTime, double dMaturity, unsigned iN)
{
  std::valarray<double> uArg = getArg (dInitialTime, dMaturity, iN + 1);
  std::vector<double> uTimes (std::begin (uArg) + 1, std::end (uArg));
  return uTimes;
}

std::valarray<double>
test::getRandArg (double dL, double dR, unsigned iN)
{
  PRECONDITION (iN > 0);

  std::valarray<double> uResult (iN);
  std::minstd_rand uGen (1);
  std::uniform_real_distribution<double> uRand (dL, dR);
  for (unsigned iI = 0; iI < iN; iI++)
    {
      uResult[iI] = uRand (uGen);
    }
  std::sort (begin (uResult), end (uResult));
  POSTCONDITION ((dL < uResult[0]) && (uResult[uResult.size () - 1] < dR));
  return uResult;
}

std::valarray<double>
test::getValues (const Function &rF, const std::valarray<double> &rArg)
{
  std::valarray<double> uResult (rArg.size ());
  std::transform (begin (rArg), end (rArg), begin (uResult),
                  [&rF] (double dX) { return rF (dX); });
  return uResult;
}

// print functions

void
test::compare (const std::valarray<double> &rExact,
               const std::valarray<double> &rNum, const std::string &rTitle,
               unsigned iColumn, unsigned iSpace, unsigned iMaxRows)
{
  PRECONDITION (rExact.size () == rNum.size ());

  std::vector<std::valarray<double> > uResults
      = { rExact, rNum, std::abs (rExact - rNum) };
  std::vector<std::string> uHeads = { "exact", "numeric", "error" };
  printTable (uResults, uHeads, rTitle, iColumn, iSpace, iMaxRows);
}

void
test::print (double dValue, const std::string &sMessage, bool bExtraLine)
{
  std::string sM (sMessage);
  std::function<double (double)> uRound = roundResult ();
  sM += std::string (" = ");
  std::cout << sM.c_str () << uRound (dValue) << endl;
  if (bExtraLine)
    {
      std::cout << endl;
    }
}

void
test::print (const std::string &sMessage, bool bExtraLine)
{
  std::cout << sMessage.c_str () << endl;
  if (bExtraLine)
    {
      std::cout << endl;
    }
}

void
test::printValues (const cfl::Function &rF, const std::valarray<double> &rArg,
                   const std::string &rTitle)
{
  std::valarray<double> uValues = getValues (rF, rArg);
  print (begin (uValues), end (uValues), rTitle);
}

void
test::printTable (const std::vector<std::valarray<double> > &rValues,
                  const std::vector<std::string> &rNames,
                  const std::string &sMessage,
                  const std::vector<unsigned> &rColumns, unsigned iSpace,
                  unsigned iMaxRows)
{
  PRECONDITION (rValues.size () == rNames.size ());
  PRECONDITION (rColumns.size () == rNames.size ());

  print (sMessage);
  for (unsigned i = 0; i < rValues.size (); i++)
    {
      std::cout << std::setw (rColumns[i]) << rNames[i].c_str ()
                << std::setw (iSpace) << "";
    }
  std::cout << endl;

  unsigned iSize = rValues.front ().size ();
  unsigned iRows = std::min (iSize, iMaxRows);
  unsigned iStart = (iSize - iRows) / 2;
  unsigned iEnd = (iSize + iRows) / 2;
  iEnd = min (iEnd, iSize);

  std::function<double (double)> uRound = roundResult ();

  for (unsigned j = iStart; j < iEnd; j++)
    {
      for (unsigned i = 0; i < rValues.size (); i++)
        {
          ASSERT (rValues[i].size () == iSize);
          std::cout << std::setw (rColumns[i]) << uRound (rValues[i][j])
                    << std::setw (iSpace) << "";
        }
      std::cout << endl;
    }
  std::cout << std::endl;
}

void
test::printTable (const std::vector<std::valarray<double> > &rValues,
                  const std::vector<std::string> &rNames,
                  const std::string &sMessage, unsigned iColumn,
                  unsigned iSpace, unsigned iMaxRows)
{
  std::vector<unsigned> uColumns (rValues.size (), iColumn);
  test::printTable (rValues, rNames, sMessage, uColumns, iSpace, iMaxRows);
}

void
test::printTable (const std::vector<std::vector<double> > &rValues,
                  const std::vector<std::string> &rNames,
                  const std::string &sMessage, unsigned iColumn,
                  unsigned iSpace, unsigned iMaxRows)
{
  unsigned iSize = rValues.front ().size ();
  std::vector<std::valarray<double> > uV (rValues.size (),
                                          std::valarray<double> (iSize));
  for (unsigned i = 0; i < rValues.size (); i++)
    {
      std::copy (rValues[i].begin (), rValues[i].end (), begin (uV[i]));
    }
  printTable (uV, rNames, sMessage, iColumn, iSpace, iMaxRows);
}

void
test::printTable (const std::vector<cfl::Function> &rF,
                  const std::vector<std::string> &rNames,
                  const std::valarray<double> &rArg,
                  const std::string &sMessage, unsigned iColumn, unsigned iArg,
                  unsigned iSpace, const std::string &sArg)
{
  PRECONDITION (rF.size () == rNames.size ());

  std::vector<std::string> uNames (rNames.size () + 1);
  uNames.front () = sArg;
  std::copy (rNames.begin (), rNames.end (), uNames.begin () + 1);

  std::vector<std::valarray<double> > uValues (
      rF.size () + 1, std::valarray<double> (rArg.size ()));
  uValues.front () = rArg;
  for (unsigned i = 0; i < rF.size (); i++)
    {
      uValues[i + 1] = getValues (rF[i], rArg);
    }

  std::vector<unsigned> uColumns (uNames.size (), iColumn);
  uColumns.front () = iArg;

  printTable (uValues, uNames, sMessage, uColumns, iSpace,
              uValues.front ().size ());
}

double
chi2 (const cfl::Function &rErr, const std::valarray<double> &rArg)
{
  std::valarray<double> uErr = test::getValues (rErr, rArg);
  double dChi2
      = std::inner_product (begin (uErr), end (uErr), begin (uErr), 0.);
  return dChi2;
}

void
test::printChi2 (const cfl::Function &rEstErr, const cfl::Function &rActErr,
                 const std::valarray<double> &rArg)
{
  print (chi2 (rEstErr, rArg), "sum of squares of estimated errors");
  print (chi2 (rActErr, rArg), "sum of squares of actual errors", true);
}

void
test::printRisk (const cfl::Function &rOption, double dRelErr, double dAbsErr,
                 double dFactor, double dShift)
{
  print ("RISK REPORT: ");
  double dCenter = 0.;
  double dL = -dShift;
  double dR = dShift;
  double dPrice = rOption (dCenter);
  auto uRound = roundResult (dRelErr, dAbsErr);
  cout << "price = " << uRound (dPrice) << endl;
  if (rOption.belongs (dR) && rOption.belongs (dL))
    {
      double dValueLeft = rOption (dL);
      double dValueRight = rOption (dR);
      double dDelta = (dValueRight - dValueLeft) / (2. * dShift);
      double dGamma = 0.01 * (dValueRight - 2. * dPrice + dValueLeft)
                      / (dShift * dShift);
      uRound = roundResult (dFactor * dRelErr, dFactor * dAbsErr);
      print (uRound (dDelta), "delta");
      uRound = roundResult (dFactor * dFactor * dRelErr,
                            dFactor * dFactor * dAbsErr);
      print (uRound (dGamma), "one percent gamma", true);
    }
}

namespace testPrint
{
void
print (const cfl::Data::CashFlow &rCashFlow, const std::string &rName)
{
  std::string sM (rName);
  sM += std::string (":");
  test::print (sM, false);
  test::print (rCashFlow.notional, "notional");
  test::print (rCashFlow.period, "period between payments");
  test::print (rCashFlow.numberOfPayments, "number of payments");
  test::print (rCashFlow.rate, "rate");
}
} // namespace testPrint

void
test::printCashFlow (const cfl::Data::CashFlow &rCashFlow,
                     const std::string &rName)
{
  testPrint::print (rCashFlow, rName);
  cout << endl;
}

void
test::printSwap (const cfl::Data::Swap &rSwap, const std::string &rName)
{
  testPrint::print (cfl::Data::CashFlow (rSwap), rName);
  if (rSwap.payFloat)
    {
      print ("we pay float and receive fixed");
    }
  else
    {
      print ("we pay fixed and receive float");
    }
}

std::function<double (double)>
test::roundResult (double dRelErr, double dAbsErr)
{
  return [dRelErr, dAbsErr] (double dX) {
    double dY = std::abs (dX);
    if (dY < dAbsErr)
      {
        return 0.;
      }

    dY *= dRelErr;

    int iN = std::floor (std::log10 (dY));
    double dNewAbsErr = std::pow (10, iN);

    ASSERT (dNewAbsErr < dY * 1.0001);
    ASSERT (dY < dNewAbsErr * 100);

    dY = std::round (dX / dNewAbsErr) * dNewAbsErr;
    return dY;
  };
}

void
test::reportAssetModel (const cfl::Function &rOption, double dSpot,
                        double dInterval, unsigned iPoints, double dRelErr,
                        double dAbsErr)
{
  test::print ("OPTION VALUES VERSUS SPOT:");

  PRECONDITION (dInterval > 0.);
  PRECONDITION (iPoints > 0);

  unsigned iSize = 2 * (iPoints / 2) + 1;
  std::vector<double> uSpot (iSize);
  std::vector<double> uOption (iSize);

  dInterval *= 0.9;

  double dX = -dInterval / 2.;
  double dStep = dInterval / (iSize - 1.);
  for (unsigned iI = 0; iI < iSize; iI++)
    {
      uSpot[iI] = std::exp (dX) * dSpot;
      uOption[iI] = rOption (dX);
      dX += dStep;
    }

  unsigned iSpot = 8;
  unsigned iSpace = 4;
  unsigned iOption = 12;

  auto uRound = test::roundResult (dRelErr, dAbsErr);
  auto uSpotRound = test::roundResult (1e-6, 1e-6);

  std::cout << std::setw (iSpot) << "spot" << std::setw (iSpace) << ""
            << std::setw (iOption) << "option" << endl;
  for (unsigned iI = 0; iI < iSize; iI++)
    {
      std::cout << std::setw (iSpot) << uSpotRound (uSpot[iI])
                << std::setw (iSpace) << "" << std::setw (iOption)
                << uRound (uOption[iI]) << endl;
    }
  std::cout << endl;
}

void
test::reportInterestRateModel (const cfl::Function &rOption, double dShortRate,
                               double dInterval, unsigned iPoints,
                               double dRelErr, double dAbsErr)
{
  test::print ("OPTION VALUES VERSUS SHORT RATE:");

  PRECONDITION (dInterval >= 0.);
  PRECONDITION (iPoints > 0);

  unsigned iSize = 2 * (iPoints / 2) + 1;
  std::vector<double> uShortRate (iSize);
  std::vector<double> uOption (iSize);

  ASSERT (iSize > 1);

  dInterval *= 0.9;
  double dX = -dInterval / 2.;
  double dStep = dInterval / (iSize - 1.);
  for (unsigned iI = 0; iI < iSize; iI++)
    {
      uShortRate[iI] = dX;
      uOption[iI] = rOption (dX);
      dX += dStep;
    }

  unsigned iRate = 6;
  unsigned iSpace = 4;
  unsigned iOption = 12;

  auto uRound = test::roundResult (dRelErr, dAbsErr);
  auto uRateRound = test::roundResult (1e-6, 1e-6);

  std::cout << std::setw (iRate) << "rate" << std::setw (iSpace) << ""
            << std::setw (iOption) << "option" << endl;
  for (unsigned iI = 0; iI < iSize; iI++)
    {
      std::cout << std::setw (iRate)
                << uRateRound (-uShortRate[iI] + dShortRate)
                << std::setw (iSpace) << "" << std::setw (iOption)
                << uRound (uOption[iI]) << endl;
    }
  std::cout << endl;
}
