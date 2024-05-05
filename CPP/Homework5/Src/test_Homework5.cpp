#include "Homework5/Homework5.hpp"
#include "Homework5/Output.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::HullWhite;

cfl::MultiFunction
collar (InterestRateModel &rModel)
{
  print ("COLLAR IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;
  double dFloorRate = test::c_dYield * 0.9;

  printCashFlow (uCapParameters, "cap parameters");
  print (dFloorRate, "floor rate", true);

  return prb::collar (uCapParameters, dFloorRate, rModel);
}

cfl::MultiFunction
americanSwaption (InterestRateModel &rModel, bool bPayFloat)
{
  print ("AMERICAN SWAPTION IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;
  std::vector<double> uExerciseTimes = test::exerciseTimes ();

  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise  times");
  printSwap (uSwap, "swap parameters");

  return prb::americanSwaption (uSwap, uExerciseTimes, rModel);
}

cfl::MultiFunction
putCallBond (InterestRateModel &rModel)
{
  print ("PUTABLE AND CALLABLE BOND IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uBondParameters = test::swapParameters ();
  double dRedemptionPrice = 0.95;
  double dRepurchasePrice = 1.05;

  printCashFlow (uBondParameters, "bond parameters");
  print (dRedemptionPrice, "redemption price for put");
  print (dRepurchasePrice, "repurchase price for call", true);

  return prb::putCallBond (uBondParameters, dRedemptionPrice, dRepurchasePrice,
                           rModel);
}

cfl::MultiFunction
americanPutOnFutures (InterestRateModel &rModel)
{
  print ("AMERICAN PUT ON FUTURES PRICE OF ZERO-COUPON BOND IN INTEREST RATE "
         "MODEL");

  double dFuturesMaturity = 0.25;
  unsigned iNumberOfFuturesTimes = 10;
  double dBondMaturity = dFuturesMaturity + 0.1;
  double dBondNotional = test::c_dNotional;
  double dDiscount
      = 1. / (1. + test::c_dYield * (dBondMaturity - dFuturesMaturity));
  double dStrike = dBondNotional * dDiscount;

  print (dFuturesMaturity, "maturity of futures");
  print (iNumberOfFuturesTimes, "number of futures times");
  print (dBondMaturity, "maturity of bond");
  print (dBondNotional, "notional of bond");
  print (dStrike, "strike", true);

  return prb::americanPutOnFutures (dBondNotional, dBondMaturity,
                                    dFuturesMaturity, iNumberOfFuturesTimes,
                                    dStrike, rModel);
}

std::function<void ()>
test_Homework5 ()
{
  return [] () {
    print ("INTEREST RATE OPTIONS IN HULL-WHITE MODEL");

    InterestRateModel uModel = test::HullWhite::model ();

    report (collar, uModel);
    report (americanSwaption, uModel);
    report (putCallBond, uModel);
    report (americanPutOnFutures, uModel);
  };
}

int
main ()
{
  project (test_Homework5 (), PROJECT_NAME, PROJECT_NAME, "Homework 5");
}
