#include "Session5/Output.hpp"
#include "Session5/Session5.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace std;
using namespace test::HullWhite;

cfl::MultiFunction
floor (InterestRateModel &rModel)
{
  print ("FLOOR IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uFloorParameters = test::swapParameters ();
  uFloorParameters.rate = test::c_dYield * 0.9;

  printCashFlow (uFloorParameters, "floor parameters");

  return prb::floor (uFloorParameters, rModel);
}

cfl::MultiFunction
putOnFRA (InterestRateModel &rModel)
{
  print ("PUT ON FORWARD RATE AGREEMENT IN INTEREST RATE MODEL");

  double dFixedRate = test::c_dYield;
  double dPeriod = test::c_dPeriod;
  double dNotional = test::c_dNotional;
  double dMaturity = test::c_dMaturity;

  print (dMaturity, "maturity");
  print (dFixedRate, "fixed rate");
  print (dPeriod, "period for FRA");
  print (dNotional, "notional", true);

  return prb::putOnFRA (dFixedRate, dPeriod, dNotional, dMaturity, rModel);
}

cfl::MultiFunction
capOnSwapRate (InterestRateModel &rModel)
{
  print ("CAP ON SWAP RATE IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;
  double dSwapPeriod = uCapParameters.period * 2;
  unsigned iSwapPayments = uCapParameters.numberOfPayments * 2;

  printCashFlow (uCapParameters, "cap parameters");
  print (dSwapPeriod, "swap period");
  print (iSwapPayments, "number of swap payments", true);

  return prb::capOnSwapRate (uCapParameters, dSwapPeriod, iSwapPayments,
                             rModel);
}

cfl::MultiFunction
cancelSwapArrears (InterestRateModel &rModel, bool bPayFloat)
{
  print ("CANCELLABLE SWAP SET IN ARREARS IN INTEREST RATE MODEL");

  Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;

  printSwap (uSwap, "swap parameters");

  return prb::cancelSwapArrears (uSwap, rModel);
}

std::function<void ()>
test_Session5 ()
{
  return [] () {
    print ("INTEREST RATE OPTIONS IN HULL-WHITE MODEL");

    InterestRateModel uModel = test::HullWhite::model ();

    report (floor, uModel);
    report (putOnFRA, uModel);
    report (capOnSwapRate, uModel);
    report (cancelSwapArrears, uModel);
  };
}

int
main ()
{
  project (test_Session5 (), PROJECT_NAME, PROJECT_NAME, "Session 5");
}
