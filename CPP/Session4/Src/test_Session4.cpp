#include "Session4/Output.hpp"
#include "Session4/Session4.hpp"
#include "test/Black.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Black;

MultiFunction
downOutCall (AssetModel &rModel)
{
  print ("DOWN-AND-OUT CALL OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dMaturity = test::c_dMaturity;
  double dLowerBarrier = test::c_dSpot * 0.9;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();

  print (dStrike, "strike");
  print (dMaturity, "maturity");
  print (dLowerBarrier, "lower barrier", true);
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::downOutCall (dLowerBarrier, uBarrierTimes, dStrike, dMaturity,
                           rModel);
}

MultiFunction
callOnForwardPrice (AssetModel &rModel)
{
  print ("CALL ON FORWARD PRICE IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dCallMaturity = test::c_dMaturity;
  double dForwardMaturity = dCallMaturity + 0.5;

  print (dStrike, "strike");
  print (dCallMaturity, "maturity of call");
  print (dForwardMaturity, "maturity of forward", true);

  return prb::callOnForwardPrice (dStrike, dCallMaturity, dForwardMaturity,
                                  rModel);
}

MultiFunction
americanButterfly (AssetModel &rModel)
{
  print ("AMERICAN BUTTERFLY OPTION IN ASSET MODEL");
  double dStrike = test::c_dSpot;
  double dStrikeStep = dStrike * 0.05;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();

  print (dStrike, "strike");
  print (dStrikeStep, "strike step", true);
  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise times");

  return prb::americanButterfly (dStrike, dStrikeStep, uExerciseTimes, rModel);
}

MultiFunction
corridor (AssetModel &rModel)
{
  print ("CORRIDOR OPTION IN ASSET MODEL");

  double dNotional = test::c_dNotional;
  double dLowerBarrier = test::c_dSpot * 0.95;
  double dUpperBarrier = test::c_dSpot * 1.1;
  const std::vector<double> uBarrierTimes = test::exerciseTimes ();

  print (dNotional, "notional");
  print (dLowerBarrier, "lower barrier");
  print (dUpperBarrier, "upper barrier", true);
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::corridor (dNotional, dLowerBarrier, dUpperBarrier, uBarrierTimes,
                        rModel);
}

std::function<void ()>
test_Session4 ()
{
  return [] () {
    print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uModel = test::Black::model ();

    report (downOutCall, uModel);
    report (callOnForwardPrice, uModel);
    report (americanButterfly, uModel);
    report (corridor, uModel);
  };
}

int
main ()
{
  project (test_Session4 (), PROJECT_NAME, PROJECT_NAME, "Session 4");
}
