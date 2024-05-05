#include "cfl/Ind.hpp"
#include "cfl/Error.hpp"
#include <algorithm>
#include <cmath>
#include <functional>

using namespace cfl;
using namespace cfl::NInd;

// class Ind

cfl::Ind::Ind (IInd *pNewInd) : m_pInd (pNewInd) {}

namespace cflInd
{
// naive indicator
class Naive : public cfl::IInd
{
public:
  void
  indicator (std::valarray<double> &rValues, double dBarrier) const
  {
    std::transform (
        begin (rValues), end (rValues), begin (rValues),
        [dBarrier] (double dX) { return (dX >= dBarrier) ? 1. : 0.; });
  }
};

// linear indicator
std::function<double (double, double)>
linearInd (double &rIndL)
{
  return [&rIndL] (double dL, double dR) {
    double dInd = rIndL;
    if (dL != dR)
      {
        rIndL = std::abs ((std::max (dL, 0.) - std::max (dR, 0.)) / (dL - dR));
      }
    else
      {
        ASSERT (dL == dR);
        rIndL = (dL >= 0) ? 1. : 0.;
      }
    return 0.5 * (dInd + rIndL);
  };
}

class Linear : public IInd
{
public:
  void
  indicator (std::valarray<double> &rValues, double dBarrier) const
  {
    rValues -= dBarrier;
    double dIndL = (rValues[0] < 0) ? 0. : 1.;

    std::transform (begin (rValues), end (rValues) - 1, begin (rValues) + 1,
                    begin (rValues), linearInd (dIndL));

    // finish at right end
    rValues[rValues.size () - 1] = 0.5 * dIndL;
    if (rValues[rValues.size () - 1] >= 0)
      {
        rValues[rValues.size () - 1] += 0.5;
      }
  }
};

// quadratic indicator
std::function<double (double, double)>
quadInd (double &rIndL)
{
  return [&rIndL] (double dL, double dR) {
    double dInd = rIndL;
    if ((dL < 0.) && (dR >= 0.))
      {
        dInd += std::pow (dR / (dR - dL), 2);
        rIndL = 1. - std::pow (dL / (dL - dR), 2);
      }
    else if ((dL >= 0) && (dR < 0))
      {
        dInd += (1. - std::pow (dR / (dR - dL), 2));
        rIndL = 1. - std::pow (dL / (dL - dR), 2);
      }
    else
      {
        rIndL = ((dL >= 0) && (dR >= 0)) ? 1. : 0.;
        dInd += rIndL;
      }
    return 0.5 * dInd;
  };
}

class Quadratic : public IInd
{
public:
  void
  indicator (std::valarray<double> &rValues, double dBarrier) const
  {
    rValues -= dBarrier;
    double dIndL = (rValues[0] < 0) ? 0. : 1.;

    std::transform (begin (rValues), end (rValues) - 1, begin (rValues) + 1,
                    begin (rValues), quadInd (dIndL));

    // finish at right end
    rValues[rValues.size () - 1] = 0.5 * dIndL;
    if (rValues[rValues.size () - 1] >= 0.)
      {
        rValues[rValues.size () - 1] += 0.5;
      }
  }
}; // namespace cflInd
} // namespace cflInd

cfl::Ind
cfl::NInd::naive ()
{
  return Ind (new cflInd::Naive ());
}

cfl::Ind
cfl::NInd::linear ()
{
  return Ind (new cflInd::Linear ());
}

cfl::Ind
cfl::NInd::quadratic ()
{
  return Ind (new cflInd::Quadratic ());
}
