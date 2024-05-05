#include "Exam/Exam.hpp"
#include "Exam/Output.hpp"
#include "test/Black.hpp"
#include "test/Data.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Data;

void
forwardSwapRate ()
{
  test::print ("FORWARD SWAP RATES");

  double dRate = 0.03;
  double dInitialTime = 1.5;
  double dPeriod = 0.25;
  unsigned iNumberOfPayments = 4;

  print (dPeriod, "swap period");
  print (iNumberOfPayments, "number of payments");
  print (dInitialTime, "initial time", true);

  Function uDiscount = cfl::Data::discount (dRate, dInitialTime);
  uDiscount += cfl::Data::discount (dRate * 4, dInitialTime);
  uDiscount *= 0.5;

  Function uForwardSwapRates
      = prb::forwardSwapRate (dPeriod, iNumberOfPayments, uDiscount);

  double dInterval = 5;
  // test::Data::print(uDiscount, dInitialTime, dInterval);
  test::Data::print (uForwardSwapRates, dInitialTime, dInterval);
}

void
discountSwapLogLinInterp ()
{
  test::print ("DISCOUNT CURVE FROM SWAP RATES BY LOG LINEAR INTERPOLATION");

  double dInitialTime = 1.;
  double dPeriod = 0.5;
  std::vector<double> uR = test::Data::getSwapRates (dPeriod, dInitialTime);

  Function uDiscount
      = prb::discountSwapLogLinInterp (uR, dPeriod, dInitialTime);

  double dInterval = uR.size () * dPeriod / 1.1;
  test::Data::print (uDiscount, dInitialTime + 0.001, dInterval);
}

MultiFunction
downRebateUpOut (AssetModel &rModel)
{
  print ("DOWN-AND-REBATE-OR-UP-AND-OUT OPTION IN ASSET MODEL");

  double dLowerBarrier = test::c_dSpot * 0.9;
  double dUpperBarrier = test::c_dSpot * 1.1;
  double dNotional = test::c_dNotional;
  const std::vector<double> uLowerBarrierTimes = test::barrierTimes ();
  const std::vector<double> uUpperBarrierTimes = test::exerciseTimes ();

  print (dLowerBarrier, "lower barrier");
  print (dUpperBarrier, "upper barrier");
  print (dNotional, "notional", true);

  print (uLowerBarrierTimes.begin (), uLowerBarrierTimes.end (),
         "lower barrier times");
  print (uUpperBarrierTimes.begin (), uUpperBarrierTimes.end (),
         "upper barrier times");

  return prb::downRebateUpOut (dNotional, dLowerBarrier, dUpperBarrier,
                               uLowerBarrierTimes, uUpperBarrierTimes, rModel);
}

cfl::MultiFunction
floorWithDeferredPayments (InterestRateModel &rModel)
{
  print ("FLOOR WITH DEFERRED PAYMENTS IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uFloorParameters = test::swapParameters ();
  printCashFlow (uFloorParameters, "floor parameters");

  return prb::floorWithDeferredPayments (uFloorParameters, rModel);
}

std::function<void ()>
test_Exam ()
{
  return [] () {
    print ("DATA CURVES FOR FINANCIAL MODELS");

    forwardSwapRate ();

    print ("INTERPOLATION OF DATA CURVES");
    // print ("LEAST-SQUARE FIT OF DATA CURVES");

    discountSwapLogLinInterp ();

    print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uBlack = test::Black::model ();

    test::report (downRebateUpOut, uBlack);

    print ("OPTIONS ON INTEREST RATES IN HULL AND WHITE MODEL");

    InterestRateModel uHullWhite = test::HullWhite::model ();

    test::report (floorWithDeferredPayments, uHullWhite);
  };
}

int
main ()
{
  project (test_Exam (), PROJECT_NAME, PROJECT_NAME, "Exam");
}
