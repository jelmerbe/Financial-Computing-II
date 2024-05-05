#include "test/HullWhite.hpp"
#include "cfl/Data.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;
using namespace cfl::HullWhite;

cfl::HullWhite::Data
test::HullWhite::data (double dYield, double dSigma, double dLambda,
                       double dInitialTime)
{
  print ("PARAMETERS OF HULL-WHITE MODEL:");
  print (dYield, "interest rate");
  print (dSigma, "sigma");
  print (dLambda, "lambda");
  print (dInitialTime, "initial time", true);

  cfl::Function uDiscount = cfl::Data::discount (dYield, dInitialTime);
  return cfl::HullWhite::makeData (uDiscount, dSigma, dLambda, dInitialTime);
}

cfl::InterestRateModel
test::HullWhite::model (double dStepQuality, double dWidthQuality)
{
  cfl::HullWhite::Data uData = test::HullWhite::data ();
  print (dStepQuality, "step quality");
  print (dWidthQuality, "width quality", true);
  return cfl::HullWhite::model (uData, c_dInterval, dStepQuality,
                                dWidthQuality);
}
