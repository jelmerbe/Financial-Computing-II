#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::put (double dStrike, double dMaturity, AssetModel &rModel)
{
  PRECONDITION (rModel.initialTime () < dMaturity);

  std::vector<double> uEventTimes = { rModel.initialTime (), dMaturity };
  rModel.assignEventTimes (uEventTimes);

  int iEventTime = 1;
  Slice uOption = max (dStrike - rModel.spot (iEventTime), 0.);
  uOption.rollback (0);

  return interpolate (uOption);
}
