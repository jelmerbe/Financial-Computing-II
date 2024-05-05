#include "ReviewExam/Output.hpp"
#include "ReviewExam/ReviewExam.hpp"
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
durationAnnuity ()
{
  test::print ("DURATION FOR AN ANNUITY");

  cfl::Data::CashFlow uAnnuity = test::swapParameters ();
  uAnnuity.notional = 1.;
  double dYTM = uAnnuity.rate * 1.1;
  double dIssueTime = 1.;
  double dInitialTime = dIssueTime + 0.5 * uAnnuity.period;
  double dMaturity = dIssueTime + uAnnuity.period * uAnnuity.numberOfPayments;
  Function uDiscount = cfl::Data::discount (dYTM, dInitialTime);
  test::print (dYTM, "yield to maturity", true);

  test::printCashFlow (uAnnuity, "annuity parameters");

  test::print (dIssueTime, "issue time");
  test::print (dInitialTime, "initial time");
  test::print (dMaturity, "maturity", true);

  double dD = prb::durationAnnuity (uAnnuity.rate, uAnnuity.period, dMaturity,
                                    dYTM, dInitialTime);
  test::print (dD, "dollar duration", true);
}

void
discountRateLogLinInterp ()
{
  test::print ("LOG LINEAR INTERPOLATION OF DISCOUNT CURVE FROM FLOAT RATES");

  double dInitialTime = 1.;
  auto uL = test::Data::getRate (dInitialTime);

  Function uDiscount
      = prb::discountRateLogLinInterp (uL.first, uL.second, dInitialTime);
  double dInterval = uL.first.back ();
  test::Data::print (uDiscount, dInitialTime, dInterval);
}

MultiFunction
swingWithPenaltyPaidAtMaturity (AssetModel &rModel)
{
  print ("SWING WITH PENALTY PAID AT MATURITY IN ASSET MODEL");

  double dStrike = test::c_dSpot;
  const std::vector<double> uExerciseTimes = test::exerciseTimes ();

  unsigned iNumberOfExercises = uExerciseTimes.size () / 3;
  double dPenalty = dStrike / 66.6;
  double dMaturity = uExerciseTimes.back () + 1. / 56.6;

  print (dStrike, "strike");
  print (iNumberOfExercises, "maximal number of exercises");
  print (dPenalty, "penalty");
  print (dMaturity, "maturity", true);

  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise times");

  return prb::swingWithPenaltyPaidAtMaturity (dStrike, uExerciseTimes,
                                              iNumberOfExercises, dPenalty,
                                              dMaturity, rModel);
}

cfl::MultiFunction
swapArrears (InterestRateModel &rModel, bool bPayFloat)
{
  print ("SWAP SET IN ARREARS IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;

  printSwap (uSwap, "swap parameters");

  return prb::swapArrears (uSwap, rModel);
}

std::function<void ()>
test_ReviewExam ()
{
  return [] () {
    print("DATA CURVES");

    durationAnnuity();
    
    print ("INTERPOLATION OF DATA CURVES");

    discountRateLogLinInterp ();

   print ("OPTIONS ON A SINGLE STOCK IN BLACK MODEL");

    AssetModel uBlack = test::Black::model ();
    test::report (swingWithPenaltyPaidAtMaturity, uBlack);

    print ("INTEREST RATE OPTIONS IN HULL-WHITE MODEL");

    InterestRateModel uHullWhite = test::HullWhite::model ();
    test::report (swapArrears, uHullWhite);
  };
}

int
main ()
{
  project (test_ReviewExam (), PROJECT_NAME, PROJECT_NAME, "ReviewExam");
}
