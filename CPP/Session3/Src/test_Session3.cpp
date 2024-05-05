#include "Session3/Output.hpp"
#include "Session3/Session3.hpp"
#include "test/Black.hpp"
#include "test/Data.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

const std::string c_sDF ("Fitted discount factors and their errors:");
const std::string c_sConstYield ("We fit with constant yield.");
const std::string c_sForward ("Fitted forward prices and their errors:");
const std::string c_sConstCarry ("We fit with constant cost-of-carry rate.");

void
discountApproxFit ()
{
  test::print ("APPROXIMATE LEAST-SQUARES FIT OF DISCOUNT CURVE");

  double dInitialTime = 1.;
  auto uDF = test::Data::getDiscount (dInitialTime);

  print (c_sConstYield);
  cfl::Fit uFit = cfl::NFit::linear (cfl::Function (1.));

  Function uErr;
  Function uDiscount = prb::discountApproxFit (uDF.first, uDF.second,
                                               dInitialTime, uFit, uErr);
  double dInterval = uDF.first.back () - dInitialTime;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sDF, uDiscount, uErr, dInitialTime, dInterval);
}

void
forwardCarryFit ()
{
  test::print ("FORWARD CURVE BY LEAST-SQUARES FIT OF COST-OF-CARRY RATES");

  double dSpot = 100.;
  double dInitialTime = 1.;

  auto uF = test::Data::getForward (dSpot, dInitialTime);

  print (c_sConstCarry);
  Fit uFit = NFit::linear (Function (1.));

  Function uErr;
  Function uForward = prb::forwardCarryFit (dSpot, uF.first, uF.second,
                                            dInitialTime, uFit, uErr);

  double dInterval = uF.first.back () - dInitialTime;
  test::Data::printFit (uFit.param ());
  test::Data::print (c_sForward, uForward, uErr, dInitialTime, dInterval);
}

void
forwardBlackFit ()
{
  test::print ("LEAST-SQUARES FIT OF FORWARD CURVE IN BLACK MODEL");

  double dSpot = test::c_dSpot;
  double dLambda = test::Black::c_dLambda;
  double dSigma = test::Black::c_dSigma;
  double dInitialTime = test::c_dInitialTime;

  print (dLambda, "lambda");
  print (dSigma, "sigma");
  auto uF = test::Data::getForward (dSpot, dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uForward = prb::forwardBlackFit (
      dSpot, uF.first, uF.second, dLambda, dSigma, dInitialTime, uErr, uParam);

  double dInterval = uF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sForward, uForward, uErr, dInitialTime, dInterval);
}

void
forwardBlackMultiFit ()
{
  test::print ("LEAST-SQUARES FIT OF FORWARD CURVE IN MULTI-DIM BLACK MODEL");

  double dSpot = test::c_dSpot;
  double dLambda = test::Black::c_dLambda;
  double dSigma = test::Black::c_dSigma;
  unsigned iSize = 3;
  std::vector<double> uLambda (iSize, dLambda);
  double dLambdaStep = dLambda / iSize;
  std::transform (uLambda.begin (), uLambda.end () - 1, uLambda.begin () + 1,
                  [dLambdaStep] (double dL) { return dL + dLambdaStep; });

  std::vector<double> uSigma (iSize, dSigma);
  double dSigmaStep = dSigma / iSize;
  std::transform (uSigma.begin (), uSigma.end () - 1, uSigma.begin () + 1,
                  [dSigmaStep] (double dL) { return dL + dSigmaStep; });

  double dInitialTime = test::c_dInitialTime;

  test::Data::print ("volatilities and mean-reversion rates:", uSigma, uLambda,
                     "sigma", "lambda");
  auto uF = test::Data::getForward (dSpot, dInitialTime);

  Function uErr;
  FitParam uParam;
  Function uForward = prb::forwardBlackMultiFit (
      dSpot, uF.first, uF.second, uLambda, uSigma, dInitialTime, uErr, uParam);
  double dInterval = uF.first.back () - dInitialTime;
  test::Data::printFit (uParam);
  test::Data::print (c_sForward, uForward, uErr, dInitialTime, dInterval);
}

std::function<void ()>
test_Session3 ()
{
  return [] () {
    print ("LEAST-SQUARES FIT OF DATA CURVES");

    discountApproxFit ();
    forwardCarryFit ();
    forwardBlackFit ();
    forwardBlackMultiFit ();
  };
}

int
main ()
{
  project (test_Session3 (), PROJECT_NAME, PROJECT_NAME, "Session 3");
}
