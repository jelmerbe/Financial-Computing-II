#include "Review3/Output.hpp"
#include "Review3/Review3.hpp"
#include "test/HullWhite.hpp"
#include "test/Main.hpp"
#include "test/Print.hpp"

using namespace test;
using namespace cfl;
using namespace prb;
using namespace std;
using namespace test::HullWhite;

cfl::MultiFunction
forwardSwapLock (InterestRateModel &rModel, bool bPayFloat)
{
  print ("FORWARD CONTRACT ON SWAP IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;
  double dMaturity = test::c_dMaturity;

  printSwap (uSwap, "swap parameters");
  print (dMaturity, "maturity", true);

  return prb::forwardSwapLock (uSwap, dMaturity, rModel);
}

cfl::MultiFunction
americanPutOnFRA (InterestRateModel &rModel)
{
  print ("AMERICAN PUT ON FORWARD RATE AGREEMENT IN INTEREST RATE MODEL");

  double dFixedRate = test::c_dYield;
  double dPeriod = test::c_dPeriod;
  double dNotional = test::c_dNotional;
  std::vector<double> uExerciseTimes = test::exerciseTimes ();

  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise  times");
  print (dFixedRate, "fixed rate");
  print (dPeriod, "period for FRA");
  print (dNotional, "notional", true);

  return prb::americanPutOnFRA (dFixedRate, dPeriod, dNotional, uExerciseTimes,
                                rModel);
}

cfl::MultiFunction
upOutFloor (InterestRateModel &rModel)
{
  print ("UP-AND-OUT FLOOR IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uFloorParameters = test::swapParameters ();
  uFloorParameters.rate = test::c_dYield * 0.9;
  double dUpperBarrier = test::c_dYield * 1.1;

  printCashFlow (uFloorParameters, "cap parameters");
  print (dUpperBarrier, "upper barrier", true);

  return prb::upOutFloor (uFloorParameters, dUpperBarrier, rModel);
}

cfl::MultiFunction
callableCappedFloater (InterestRateModel &rModel)
{
  print ("CALLABLE CAPPED FLOATER IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uCapParameters = test::swapParameters ();
  uCapParameters.rate = test::c_dYield * 1.1;
  double dRateSpread = 0.01;

  printCashFlow (uCapParameters, "cap parameters");
  print (dRateSpread, "spread over float rate", true);

  return prb::callableCappedFloater (uCapParameters, dRateSpread, rModel);
}

cfl::MultiFunction
futuresOnCheapToDeliver (InterestRateModel &rModel)
{
  print ("FUTURES PRICE OF CHEAPEST TO DELIVER COUPON BOND IN INTEREST RATE "
         "MODEL");

  double dFuturesMaturity = 0.5;
  unsigned iNumberOfFuturesTimes = 20;

  print (dFuturesMaturity, "maturity of futures contract");
  print (iNumberOfFuturesTimes, "number of futures times", true);

  unsigned iNumberOfBonds = 5;
  std::vector<cfl::Data::CashFlow> uBonds (iNumberOfBonds);
  double dNotional = test::c_dNotional;
  double dRate = test::c_dYield;
  double dPeriod = 0.25;
  unsigned iNumberOfPeriods = 4;
  for (unsigned i = 0; i < iNumberOfBonds; i++)
    {
      uBonds[i].notional = dNotional;
      uBonds[i].rate = dRate;
      uBonds[i].period = dPeriod;
      uBonds[i].numberOfPayments = iNumberOfPeriods + i;

      print (i, "bond index");
      print (uBonds[i].notional, "notional");
      print (uBonds[i].rate, "coupon rate");
      print (uBonds[i].period, "period");
      print (uBonds[i].numberOfPayments, "number of payments", true);
    }

  return prb::futuresOnCheapToDeliver (dFuturesMaturity, iNumberOfFuturesTimes,
                                       uBonds, rModel);
}

cfl::MultiFunction
resetCouponPutBond (InterestRateModel &rModel)
{
  print ("PUTABLE BOND WITH RESETTABLE COUPON IN INTEREST RATE MODEL");

  cfl::Data::CashFlow uBondParameters = test::swapParameters ();
  double dResetCouponRate = uBondParameters.rate - 0.02;
  double dRedemptionPrice = 0.98;

  printCashFlow (uBondParameters, "bond parameters");
  print (dResetCouponRate, "reset value for coupon rate");
  print (dRedemptionPrice, "redemption price (percent of notional)", true);

  return prb::resetCouponPutBond (uBondParameters, dResetCouponRate,
                                  dRedemptionPrice, rModel);
}

cfl::MultiFunction
CMSwaption (InterestRateModel &rModel, bool bPayFloat)
{
  print ("CONSTANT MATURITY SWAPTION IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;
  double dMaturity = rModel.initialTime () + 0.5;
  unsigned iPeriods = 3;

  printSwap (uSwap, "swap parameters");
  print (dMaturity, "maturity");
  print (iPeriods, "number of periods in CMS swap rate", true);

  return prb::CMSwaption (uSwap, dMaturity, iPeriods, rModel);
}

cfl::MultiFunction
americanCMSwaption (InterestRateModel &rModel, bool bPayFloat)
{
  print ("AMERICAN CONSTANT MATURITY SWAPTION IN INTEREST RATE MODEL");

  cfl::Data::Swap uSwap = test::swapParameters ();
  uSwap.payFloat = bPayFloat;
  std::vector<double> uExerciseTimes = test::exerciseTimes ();
  unsigned iCMSPeriods = 3;

  print (iCMSPeriods, "CMS periods", true);
  print (uExerciseTimes.begin (), uExerciseTimes.end (), "exercise  times");
  printSwap (uSwap, "swap parameters");

  return prb::americanCMSwaption (uSwap, uExerciseTimes, iCMSPeriods, rModel);
}

std::function<void ()>
test_Review3 ()
{
  return [] () {
    print ("INTEREST RATE OPTIONS IN HULL-WHITE MODEL");

    InterestRateModel uModel = test::HullWhite::model ();

    report (forwardSwapLock, uModel);
    report (americanPutOnFRA, uModel);
    report (upOutFloor, uModel);
    report (callableCappedFloater, uModel);
    report (futuresOnCheapToDeliver, uModel);
    report (resetCouponPutBond, uModel);
    report (CMSwaption, uModel);
    report (americanCMSwaption, uModel);
  };
}

int
main ()
{
  project (test_Review3 (), PROJECT_NAME, PROJECT_NAME, "Review 3");
}
