#include "Examples/Examples.hpp"
#include <numeric>

using namespace cfl;
using namespace std;

cfl::MultiFunction
prb::forwardOnAverageSpotDetermRates (const std::vector<double> &rAverTimes,
                                      cfl::AssetModel &rModel)
{
  PRECONDITION (rAverTimes.front () > rModel.initialTime ());
  PRECONDITION (std::is_sorted (rAverTimes.begin (), rAverTimes.end (),
                                std::less_equal<double> ()));

  std::vector<double> uEventTimes (1, rModel.initialTime ());

  Slice uOption = std::accumulate (
      rAverTimes.begin (), rAverTimes.end (), rModel.cash (0, 0.),
      [&rModel] (const Slice &rSum, double dMaturity) {
        return rSum + rModel.forward (0, dMaturity);
      });
  uOption /= rAverTimes.size ();

  return interpolate (uOption);
}
