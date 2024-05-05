#include "Homework4/Homework4.hpp"
#include "Homework4/Output.hpp"
#include "test/Black.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Black;

MultiFunction
straddle (AssetModel &rModel)
{
  print ("EUROPEAN STRADDLE OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dMaturity = test::c_dMaturity;

  print (dStrike, "strike");
  print (dMaturity, "maturity", true);

  return prb::straddle (dStrike, dMaturity, rModel);
}

MultiFunction
americanCallOnForward (AssetModel &rModel)
{
  print ("AMERICAN CALL ON FORWARD IN ASSET MODEL");

  double dForwardPrice = test::c_dSpot;
  double dTimeToMaturity = 0.5;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();

  print (dForwardPrice, "forward price");
  print (dTimeToMaturity, "time to maturity", true);
  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise times");

  return prb::americanCallOnForward (dForwardPrice, dTimeToMaturity,
                                     uExerciseTimes, rModel);
}

MultiFunction
downRebate (AssetModel &rModel)
{
  print ("DOWN-AND-REBATE OPTION IN ASSET MODEL");

  double dLowerBarrier = test::c_dSpot * 0.9;
  double dNotional = test::c_dNotional;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();

  print (dLowerBarrier, "lower barrier");
  print (dNotional, "notional", true);
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::downRebate (dLowerBarrier, dNotional, uBarrierTimes, rModel);
}

MultiFunction
upInAmericanPut (AssetModel &rModel)
{
  print ("UP-AND-IN AMERICAN PUT OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();
  double dUpperBarrier = test::c_dSpot * 1.1;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();

  print (dStrike, "strike");
  print (dUpperBarrier, "upper barrier", true);
  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise times");
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::upInAmericanPut (dUpperBarrier, uBarrierTimes, dStrike,
                               uExerciseTimes, rModel);
}

std::function<void ()>
test_Homework4 ()
{
  return [] () {
    print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uModel = test::Black::model ();

    report (straddle, uModel);
    report (americanCallOnForward, uModel);
    report (downRebate, uModel);
    report (upInAmericanPut, uModel);
  };
}

int
main ()
{
  project (test_Homework4 (), PROJECT_NAME, PROJECT_NAME, "Homework 4");
}
