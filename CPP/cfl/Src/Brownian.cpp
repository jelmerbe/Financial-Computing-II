#include "cfl/Brownian.hpp"
#include "cfl/GaussRollback.hpp"
#include "cfl/Ind.hpp"
#include "cfl/Interp.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

using namespace cfl;

namespace cflBrownian
{
class Model : public cfl::IModel
{
public:
  Model (const std::function<double (double)> &rH,
         const std::function<double (double)> &rWidth,
         const std::function<unsigned (double)> &rSize,
         const GaussRollback &rRollback, const Ind &rInd,
         const Interp &rInterp, const std::vector<double> &rVar,
         const std::vector<double> &rEventTimes, double dInterval);

  const std::vector<double> &
  eventTimes () const
  {
    return m_uEventTimes;
  }

  unsigned
  numberOfStates () const
  {
    return 1;
  }

  unsigned
  numberOfNodes (unsigned iTime,
                 const std::vector<unsigned> &rDependence) const
  {
    PRECONDITION (rDependence.size () <= 1);

    if (rDependence.size () == 0)
      {
        return 1;
      }
    else
      {
        ASSERT (rDependence.front () == 0);

        return m_uSize[iTime];
      }
  }

  std::valarray<double>
  origin () const
  {
    return std::valarray<double> (0., 1);
  }

  Slice state (unsigned iTime, unsigned iState) const;

  void addDependence (Slice &rSlice,
                      const std::vector<unsigned> &rDependence) const;

  void rollback (Slice &rSlice, unsigned iTime) const;

  void
  indicator (Slice &rSlice, double dBarrier) const
  {
    m_uInd.indicator (rSlice.values (), dBarrier);
  }

  MultiFunction interpolate (const Slice &rSlice) const;

private:
  std::function<double (double)> m_uWidth;
  GaussRollback m_uGaussRollback;
  Ind m_uInd;
  Interp m_uInterp;
  std::vector<double> m_uTotalVar, m_uEventTimes;
  std::vector<unsigned> m_uSize;
  double m_dH;
};
} // namespace cflBrownian

using namespace cflBrownian;

// CLASS cflBrownian::Model

double
minVar (const std::vector<double> &rVar)
{
  double dMinVar = std::inner_product (
      rVar.begin () + 1, rVar.end (), rVar.begin (), cfl::OMEGA,
      [] (double dX, double dY) { return std::min (dX, dY); },
      std::minus<double> ());

  ASSERT (dMinVar > cfl::EPS);

  return dMinVar;
}

cflBrownian::Model::Model (const std::function<double (double)> &rH,
                           const std::function<double (double)> &rWidth,
                           const std::function<unsigned (double)> &rSize,
                           const GaussRollback &rRollback, const Ind &rInd,
                           const Interp &rInterp,
                           const std::vector<double> &rVar,
                           const std::vector<double> &rEventTimes,
                           double dInterval)
    : m_uWidth (rWidth), m_uGaussRollback (rRollback), m_uInd (rInd),
      m_uInterp (rInterp), m_uTotalVar (rVar.size ()),
      m_uEventTimes (rEventTimes), m_uSize (rEventTimes.size ())

{
  PRECONDITION (rEventTimes.size () == rVar.size ());
  PRECONDITION (std::equal (m_uEventTimes.begin () + 1, m_uEventTimes.end (),
                            m_uEventTimes.begin (), std::greater<double> ()));

  double dToday = rEventTimes.front ();
  std::transform (rVar.begin (), rVar.end (), rEventTimes.begin (),
                  m_uTotalVar.begin (), [dToday] (double dVar, double dTime) {
                    ASSERT (dTime >= dToday);
                    return dVar * (dTime - dToday);
                  });

  ASSERT (std::equal (m_uTotalVar.begin () + 1, m_uTotalVar.end (),
                      m_uTotalVar.begin (), std::greater<double> ()));

  double dMinVar = minVar (m_uTotalVar);
  m_dH = rH (dMinVar);

  std::transform (m_uTotalVar.begin (), m_uTotalVar.end (), m_uSize.begin (),
                  [&rSize, &rWidth, dH = m_dH, dInterval] (double dVar) {
                    double dW = rWidth (dVar);

                    ASSERT (dW > 0);

                    double dSize
                        = std::max ((dInterval + dW) / dH, 2.) + cfl::EPS;
                    unsigned iSize = rSize (dSize);

                    ASSERT (iSize * dH > dInterval + dW);

                    return iSize;
                  });

  POSTCONDITION (std::equal (m_uSize.begin () + 1, m_uSize.end (),
                             m_uSize.begin (),
                             std::greater_equal<unsigned> ()));
}

Slice
cflBrownian::Model::state (unsigned iTime, unsigned iState) const
{

  PRECONDITION (iState == 0);

  std::vector<unsigned> uDependence (1, 0);

  unsigned iSize = m_uSize[iTime];
  std::valarray<double> uValues (iSize);
  uValues[0] = -m_dH * (iSize - 1) / 2.;

  std::transform (begin (uValues), end (uValues) - 1, begin (uValues) + 1,
                  [dH = m_dH] (double dX) { return dH + dX; });

  return Slice (*this, iTime, uDependence, uValues);
}

void
cflBrownian::Model::addDependence (
    Slice &rSlice, const std::vector<unsigned> &rDependence) const
{
  PRECONDITION (rDependence.size () <= 1);

  if ((rSlice.dependence ().size () == 0) && (rDependence.size () == 1))
    {
      ASSERT (rSlice.values ().size () == 1);

      std::valarray<double> uValues (rSlice.values ()[0],
                                     m_uSize[rSlice.timeIndex ()]);
      rSlice.assign (rDependence, uValues);
    }
}

void
cflBrownian::Model::rollback (Slice &rSlice, unsigned iTime) const
{
  PRECONDITION (rSlice.dependence ().size () <= 1);
  PRECONDITION (&rSlice.model () == this);
  PRECONDITION (rSlice.timeIndex () > iTime);

  double dVar = m_uTotalVar[rSlice.timeIndex ()] - m_uTotalVar[iTime];

  ASSERT (dVar > VAR_EPS);

  std::valarray<double> &rValues = rSlice.values ();

  ASSERT (rValues.size () > 0);

  bool bRollback = (rValues.size () > 1);

  if (bRollback)
    {
      ASSERT (m_dH * m_dH <= 1.5001 * dVar); // at least one uniform step

      GaussRollback uRoll (m_uGaussRollback);
      uRoll.assign (rValues.size (), m_dH, dVar);
      uRoll.rollback (rValues);
    }

  unsigned iSize1 = numberOfNodes (iTime, rSlice.dependence ());

  ASSERT (iSize1 <= rValues.size ());

  if (iSize1 < rValues.size ())
    {
      unsigned iI = (rValues.size () - iSize1) / 2;
      std::valarray<double> uT (rValues[std::slice (iI, iSize1, 1)]);
      rSlice.assign (iTime, rSlice.dependence (), uT);
    }
  else
    {
      rSlice.assign (iTime, rSlice.dependence (), rValues);
    }
}

MultiFunction
cflBrownian::Model::interpolate (const Slice &rSlice) const
{
  Slice uState = state (rSlice.timeIndex (), 0);
  const std::valarray<double> &rArg = uState.values ();
  const std::valarray<double> &rVal = rSlice.values ();
  Interp uInterp (m_uInterp);
  uInterp.assign (std::begin (rArg), std::end (rArg), std::begin (rVal));
  Function uF = uInterp.interp ();

  return MultiFunction (uF);
}

// constructor of model for Brownian motion

cfl::TBrownian
cfl::brownian (const std::function<double (double)> &rH,
               const std::function<double (double)> &rWidth,
               const std::function<unsigned (double)> &rSize,
               const GaussRollback &rRollback, const Ind &rInd,
               const Interp &rInterp)
{
  return [rH, rWidth, rSize, rRollback, rInd,
          rInterp] (const std::vector<double> &rVar,
                    const std::vector<double> &rEventTimes, double dInterval) {
    return cfl::Model (new cflBrownian::Model (rH, rWidth, rSize, rRollback,
                                               rInd, rInterp, rVar,
                                               rEventTimes, dInterval));
  };
}

cfl::TBrownian
cfl::brownian (double dStepQuality, double dWidthQuality,
               unsigned iUniformSteps,
               const std::function<unsigned (double)> &rSize,
               const GaussRollback &rRollback, const Ind &rInd,
               const Interp &rInterp)
{
  return brownian (Grid::step (dStepQuality, iUniformSteps),
                   Grid::widthGauss (dWidthQuality), rSize, rRollback, rInd,
                   rInterp);
}
