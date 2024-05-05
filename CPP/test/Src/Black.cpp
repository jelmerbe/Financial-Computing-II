#include "test/Black.hpp"
#include "cfl/Data.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;
using namespace cfl::Black;

cfl::Black::Data
test::Black::data (const char *sModel, double dYield, double dSpot,
                   double dDividendYield, double dSigma, double dLambda,
                   double dInitialTime)
{
  print (sModel);
  print (dYield, "interest rate");
  print (dSpot, "spot price");
  print (dDividendYield, "convenience yield");
  print (dSigma, "sigma");
  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);

  cfl::Function uDiscount = cfl::Data::discount (dYield, dInitialTime);
  cfl::Function uForward
      = cfl::Data::forward (dSpot, dDividendYield, uDiscount, dInitialTime);
  return cfl::Black::makeData (uDiscount, uForward, dSigma, dLambda,
                               dInitialTime);
}

cfl::AssetModel
test::Black::model (double dStepQuality, double dWidthQuality)
{
  cfl::Black::Data uData = test::Black::data ();
  print (dStepQuality, "step quality");
  print (dWidthQuality, "width quality", true);
  return cfl::Black::model (uData, c_dInterval, dStepQuality, dWidthQuality);
}
