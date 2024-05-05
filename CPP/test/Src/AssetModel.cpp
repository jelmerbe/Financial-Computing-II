#include "test/AssetModel.hpp"
#include "test/Main.hpp"
#include "test/Output.hpp"

using namespace cfl;
using namespace std;
using namespace test;

void
test::report (MultiFunction (*f) (AssetModel &rModel), AssetModel &rModel,
              double dRelErr, double dAbsErr)
{
  Function uOption = toFunction (f (rModel));
  printRisk (uOption, dRelErr, dAbsErr);
  reportAssetModel (uOption, c_dSpot, c_dInterval, c_iPoints, dRelErr,
                    dAbsErr);
}

void
test::report (MultiFunction (*f) (AssetModel &rModel, bool bPayFloat),
              AssetModel &rModel, double dRelErr, double dAbsErr)
{
  for (unsigned i = 0; i < 2; i++)
    {
      bool bPayFloat = (i == 0) ? true : false;
      Function uOption = toFunction (f (rModel, bPayFloat));
      printRisk (uOption, dRelErr, dAbsErr);
      reportAssetModel (uOption, c_dSpot, c_dInterval, c_iPoints, dRelErr,
                        dAbsErr);
    }
}
