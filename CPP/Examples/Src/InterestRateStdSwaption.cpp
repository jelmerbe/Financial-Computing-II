#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::swaption (const Data::Swap &rSwap, double dMaturity,
               InterestRateModel &rModel)
{
  PRECONDITION (rModel.initialTime () < dMaturity);

  std::vector<double> uEventTimes = { rModel.initialTime (), dMaturity };
  rModel.assignEventTimes (uEventTimes);

  int iTime = 1;
  Slice uOption = max (NExamples::swap (iTime, rSwap, rModel), 0.);
  uOption.rollback (0);

  return interpolate (uOption);
}
