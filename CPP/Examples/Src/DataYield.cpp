#include "Examples/Examples.hpp"
#include "cfl/Macros.hpp"
#include <cmath>

using namespace cfl;
using namespace cfl::Data;

class Yield : public cfl::IFunction
{
public:
  Yield (const cfl::Function &rDiscount, double dInitialTime)
      : m_uDiscount (rDiscount), m_dInitialTime (dInitialTime)
  {
  }

  double
  operator() (double dT) const
  {
    PRECONDITION (belongs (dT));

    double dYield;
    if (dT < m_dInitialTime + cfl::EPS)
      {
        dYield = (1. - m_uDiscount (m_dInitialTime + cfl::EPS)) / cfl::EPS;
      }
    else
      {
        dYield = -std::log (m_uDiscount (dT)) / (dT - m_dInitialTime);
      }

    return dYield;
  }

  bool
  belongs (double dT) const
  {
    return m_uDiscount.belongs (dT);
  }

private:
  cfl::Function m_uDiscount;
  double m_dInitialTime;
};

cfl::Function
prb::yield (const cfl::Function &rDiscount, double dInitialTime)
{
  return cfl::Function (new Yield (rDiscount, dInitialTime));
}
