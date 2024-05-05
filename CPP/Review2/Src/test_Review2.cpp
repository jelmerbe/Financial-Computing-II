#include "Review2/Output.hpp"
#include "Review2/Review2.hpp"
#include "test/Black.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::Black;

MultiFunction
boost (AssetModel &rModel)
{
  print ("BOOST OPTION IN ASSET MODEL");

  double dNotional = test::c_dNotional;
  double dLowerBarrier = test::c_dSpot * 0.95;
  double dUpperBarrier = test::c_dSpot * 1.1;
  const std::vector<double> uBarrierTimes = test::exerciseTimes ();

  print (dNotional, "notional");
  print (dLowerBarrier, "lower barrier");
  print (dUpperBarrier, "upper barrier", true);
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::boost (dNotional, dLowerBarrier, dUpperBarrier, uBarrierTimes,
                     rModel);
}

MultiFunction
clique (AssetModel &rModel)
{
  print ("CLIQUE OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  std::vector<double> uAverageTimes = test::exerciseTimes ();
  double dMaturity = uAverageTimes.back () + 5. / 256.; // one week later

  print (dStrike, "strike");
  print (dMaturity, "maturity", true);
  print (uAverageTimes.begin (), uAverageTimes.end (), "averaging times");

  return prb::clique (dMaturity, uAverageTimes, dStrike, rModel);
}

MultiFunction
simpleSwingWithPenalty (AssetModel &rModel)
{
  print ("SIMPLE SWING OPTION WITH PENALTY IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dPenalty = 0.05 * dStrike;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();
  double dMaturity = uExerciseTimes.back () + 0.1;

  print (dStrike, "strike");
  print (dPenalty, "penalty");
  print (dMaturity, "maturity");
  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise times");

  return prb::simpleSwingWithPenalty (dStrike, dPenalty, uExerciseTimes,
                                      dMaturity, rModel);
}

MultiFunction
americanChooser (AssetModel &rModel)
{
  print ("AMERICAN CHOOSER OPTION IN ASSET MODEL");

  const std::vector<double> uExerciseTimes = test::exerciseTimes ();
  double dMaturity = uExerciseTimes.back () + 0.1;
  double dPutStrike = test::c_dSpot * 0.9;
  double dCallStrike = test::c_dSpot * 1.1;

  ASSERT (dPutStrike < dCallStrike);

  std::vector<double> uPutStrikes (uExerciseTimes.size (), dPutStrike);
  std::vector<double> uCallStrikes (uExerciseTimes.size (), dCallStrike);

  for (unsigned iI = 0; iI < uPutStrikes.size (); iI++)
    {
      uPutStrikes[iI] = dPutStrike * (1. + 0.1 * std::sin (iI + 0.1));
      uCallStrikes[iI] = dCallStrike * (1. + 0.1 * std::sin (iI + 0.1));
    }

  std::vector<std::string> uTitle
      = { "exercise times", "put strikes", "call strikes" };
  std::vector<std::vector<double> > uValues
      = { uExerciseTimes, uPutStrikes, uCallStrikes };
  printTable (uValues, uTitle, "timeline of put and calls:", 12);
  print (dMaturity, "maturity", true);

  return prb::americanChooser (uPutStrikes, uCallStrikes, uExerciseTimes,
                               dMaturity, rModel);
}

MultiFunction
upRangeOutPut (AssetModel &rModel)
{
  print ("UP-RANGE-OUT PUT OPTION IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  double dMaturity = test::c_dMaturity;
  double dUpperBarrier = test::c_dSpot * 1.02;
  const std::vector<double> uBarrierTimes = test::barrierTimes ();
  unsigned iOutTimes = std::min<int> (uBarrierTimes.size (), 4);

  print (dStrike, "strike");
  print (dMaturity, "maturity");
  print (dUpperBarrier, "upper barrier");
  print (iOutTimes, "number of barrier events needed to cancel put", true);
  print (uBarrierTimes.begin (), uBarrierTimes.end (), "barrier times");

  return prb::upRangeOutPut (dUpperBarrier, iOutTimes, uBarrierTimes, dStrike,
                             dMaturity, rModel);
}

MultiFunction
varianceSwapStrike (AssetModel &rModel)
{
  print ("STRIKE OF VARIANCE SWAP IN ASSET MODEL");

  double dMaturity = test::c_dMaturity;
  unsigned iNumberOfTimes = 12;

  print (dMaturity, "maturity");
  print (iNumberOfTimes, "number of variance times", true);

  return prb::varianceSwapStrike (dMaturity, iNumberOfTimes, rModel);
}

MultiFunction
assetSwaption (AssetModel &rModel, bool bPayFloat)
{
  print ("ASSET SWAPTION IN ASSET MODEL");

  double dMaturity = test::c_dMaturity;
  Data::Swap uSwap;
  uSwap.notional = 10000;
  uSwap.period = 0.25;
  uSwap.numberOfPayments = 6;
  uSwap.rate = -test::c_dDividendYield;
  uSwap.payFloat = bPayFloat;

  print (uSwap.notional, "notional");
  print (uSwap.period, "period between two payments");
  print (uSwap.numberOfPayments, "number of payments");
  print (uSwap.rate, "spread over float rate");

  if (uSwap.payFloat)
    {
      print ("we pay float interest plus spread and receive asset returns");
    }
  else
    {
      print ("we receive float interest plus spread and pay asset returns");
    }

  return prb::assetSwaption (uSwap, dMaturity, rModel);
}

MultiFunction
fxCancellableSwap (AssetModel &rModel)
{
  print ("FX CANCELLABLE SWAP IN ASSET MODEL");

  double dDomNotional = 10000;
  double dDomFixedRate = test::c_dYield;
  double dForNotional = dDomNotional / atOrigin (rModel.spot (0))[0];
  double dForRateSpread = 0.005;
  double dPeriod = 0.25;
  unsigned iNumberOfPayments = 4;

  print (dDomNotional, "domestic notional");
  print (dDomFixedRate, "domestic fixed rate");
  print (dForNotional, "foreign notional");
  print (dForRateSpread, "spread for foreign float rate");
  print (dPeriod, "period between payments");
  print (iNumberOfPayments, "number of payments", true);

  return prb::fxCancellableSwap (dDomNotional, dDomFixedRate, dForNotional,
                                 dForRateSpread, dPeriod, iNumberOfPayments,
                                 rModel);
}

std::function<void ()>
test_Review2 ()
{
  return [] () {
    print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uModel = test::Black::model ();
    report (boost, uModel);
    report (clique, uModel);
    report (simpleSwingWithPenalty, uModel);
    report (americanChooser, uModel);
    report (upRangeOutPut, uModel);
    report (varianceSwapStrike, uModel);
    report (assetSwaption, uModel);
    report (fxCancellableSwap, uModel);
  };
}

int
main ()
{
  project (test_Review2 (), PROJECT_NAME, PROJECT_NAME, "Review 2");
}
