#include "test/InterestRateModel.hpp"
#include "test/Main.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;

void
test::report (MultiFunction (*f) (InterestRateModel &rModel),
              InterestRateModel &rModel, double dRelErr, double dAbsErr)
{
  Function uOption = toFunction (f (rModel));
  printRisk (uOption, dRelErr, dAbsErr);
  reportInterestRateModel (uOption, c_dYield, c_dInterval, c_iPoints, dRelErr,
                           dAbsErr);
}

void
test::report (MultiFunction (*f) (InterestRateModel &rModel, bool bPayFloat),
              InterestRateModel &rModel, double dRelErr, double dAbsErr)
{
  for (unsigned i = 0; i < 2; i++)
    {
      bool bPayFloat = (i == 0) ? true : false;
      Function uOption = toFunction (f (rModel, bPayFloat));
      printRisk (uOption, dRelErr, dAbsErr);
      reportInterestRateModel (uOption, c_dYield, c_dInterval, c_iPoints,
                               dRelErr, dAbsErr);
    }
}
