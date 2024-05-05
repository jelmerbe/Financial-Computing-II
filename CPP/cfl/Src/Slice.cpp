#include "cfl/Slice.hpp"
#include "cfl/Error.hpp"
#include <algorithm>
#include <cmath>

using namespace cfl;
using namespace std;

// constructors

cfl::Slice::Slice (const IModel *pModel, unsigned iTime, double dValue)
    : m_pModel (pModel), m_iTime (iTime), m_uValues (dValue, 1)
{
  POSTCONDITION (m_uDependence.size () == 0);
}

cfl::Slice::Slice (const cfl::IModel &rModel, unsigned iTime,
                   const vector<unsigned> &rDependence,
                   const valarray<double> &rValues)
    : m_pModel (&rModel), m_iTime (iTime), m_uDependence (rDependence),
      m_uValues (rValues)
{
  POSTCONDITION (rValues.size ()
                 == m_pModel->numberOfNodes (iTime, rDependence));
}

// member functions

namespace cflSlice
{
void
apply (Slice &rS1, const Slice &rS2,
       const function<void (valarray<double> &, const valarray<double>)> &rF)
{
  PRECONDITION (&rS1.model () == &rS2.model ());
  PRECONDITION (rS1.timeIndex () == rS2.timeIndex ());

  const IModel &rModel = rS1.model ();
  const vector<unsigned> &rD1 = rS1.dependence ();
  const vector<unsigned> &rD2 = rS2.dependence ();

  if ((rD1.size () == rD2.size ())
      && (equal (rD1.begin (), rD1.end (), rD2.begin ())))
    {
      rF (rS1.values (), rS2.values ());
    }
  else if ((rD1.size () > rD2.size ())
           && (includes (rD1.begin (), rD1.end (), rD2.begin (), rD2.end ())))
    {
      Slice uSlice (rS2);
      rModel.addDependence (uSlice, rD1);
      rF (rS1.values (), uSlice.values ());
    }
  else if ((rD2.size () > rD1.size ())
           && (includes (rD2.begin (), rD2.end (), rD1.begin (), rD1.end ())))
    {
      rModel.addDependence (rS1, rD2);
      rF (rS1.values (), rS2.values ());
    }
  else
    {
      rModel.addDependence (rS1, rD2);
      Slice uSlice (rS2);
      rModel.addDependence (uSlice, rD1);
      rF (rS1.values (), uSlice.values ());
    }
}
} // namespace cflSlice

Slice &
cfl::Slice::operator+= (const Slice &rSlice)
{
  PRECONDITION (m_pModel == &rSlice.model ());
  PRECONDITION (timeIndex () == rSlice.timeIndex ());

  if (rSlice.values ().size () == 1)
    {
      return operator+= (rSlice.values ()[0]);
    }

  cflSlice::apply (
      *this, rSlice,
      [] (valarray<double> &rS1, const valarray<double> &rS2) { rS1 += rS2; });

  return *this;
}

Slice &
cfl::Slice::operator-= (const Slice &rSlice)
{
  PRECONDITION (m_pModel == &rSlice.model ());
  PRECONDITION (timeIndex () == rSlice.timeIndex ());

  if (rSlice.values ().size () == 1)
    {
      return operator-= (rSlice.values ()[0]);
    }

  cflSlice::apply (
      *this, rSlice,
      [] (valarray<double> &rS1, const valarray<double> &rS2) { rS1 -= rS2; });

  return *this;
}

Slice &
cfl::Slice::operator*= (const Slice &rSlice)
{
  PRECONDITION (m_pModel == &rSlice.model ());
  PRECONDITION (timeIndex () == rSlice.timeIndex ());

  if (rSlice.values ().size () == 1)
    {
      return operator*= (rSlice.values ()[0]);
    }

  cflSlice::apply (
      *this, rSlice,
      [] (valarray<double> &rS1, const valarray<double> &rS2) { rS1 *= rS2; });

  return *this;
}

Slice &
cfl::Slice::operator/= (const Slice &rSlice)
{
  PRECONDITION (m_pModel == &rSlice.model ());
  PRECONDITION (timeIndex () == rSlice.timeIndex ());

  if (rSlice.values ().size () == 1)
    {
      return operator/= (rSlice.values ()[0]);
    }

  cflSlice::apply (
      *this, rSlice,
      [] (valarray<double> &rS1, const valarray<double> &rS2) { rS1 /= rS2; });

  return *this;
}

// global functions

Slice
cfl::max (const Slice &rA, double dV)
{
  Slice uR (rA);
  transform (begin (rA.values ()), end (rA.values ()), begin (uR.values ()),
             [dV] (double dA) { return std::max (dA, dV); });

  return uR;
}

Slice
cfl::min (const Slice &rA, double dV)
{
  Slice uR (rA);
  transform (begin (rA.values ()), end (rA.values ()), begin (uR.values ()),
             [dV] (double dA) { return std::min (dA, dV); });

  return uR;
}

Slice
cfl::max (const Slice &rS1, const Slice &rS2)
{
  Slice uR (rS1);
  cflSlice::apply (uR, rS2,
                   [] (valarray<double> &rS1, const valarray<double> &rS2) {
                     transform (begin (rS1), end (rS1), begin (rS2),
                                begin (rS1), [] (double dS1, double dS2) {
                                  return std::max (dS1, dS2);
                                });
                   });

  return uR;
}

Slice
cfl::min (const Slice &rS1, const Slice &rS2)
{
  Slice uR (rS1);
  cflSlice::apply (uR, rS2,
                   [] (valarray<double> &rS1, const valarray<double> &rS2) {
                     transform (begin (rS1), end (rS1), begin (rS2),
                                begin (rS1), [] (double dS1, double dS2) {
                                  return std::min (dS1, dS2);
                                });
                   });

  return uR;
}

MultiFunction
cfl::interpolate (const Slice &rSlice, const vector<unsigned> &rState)
{
  const IModel &rModel = rSlice.model ();

  PRECONDITION (rModel.numberOfStates () >= rSlice.dependence ().size ());
  PRECONDITION (
      is_sorted (rState.begin (), rState.end (), less_equal<size_t> ()));
  PRECONDITION (rState.size () > 0);
  PRECONDITION (rState.back () < rModel.numberOfStates ());

  Slice uSlice (rSlice);
  rModel.addDependence (uSlice, rState);
  const vector<unsigned> &rIx = uSlice.dependence ();

  ASSERT (std::includes (rIx.begin (), rIx.end (), rState.begin (),
                         rState.end ()));

  MultiFunction uF = interpolate (uSlice);

  ASSERT (uF.dimD () == rIx.size ());

  if (rIx.size () == rState.size ())
    {
      return uF;
    }

  valarray<size_t> uFixedIx (rIx.size () - rState.size ());

  ASSERT (uFixedIx.size () > 0);

  std::set_difference (rIx.begin (), rIx.end (), rState.begin (),
                       rState.end (), begin (uFixedIx));
  valarray<double> uPoint (rModel.origin ()[uFixedIx]);

  ASSERT (uF.dimD () == rState.size () + uPoint.size ());

  valarray<size_t> uStateIx (rState.size ());
  auto itI = rIx.begin ();
  std::transform (rState.begin (), rState.end (), begin (uStateIx),
                  [&itI, &rIx] (unsigned iState) {
                    itI = std::lower_bound (itI, rIx.end (), iState);
                    return itI - rIx.begin ();
                  });

  return section (uF, uStateIx, uPoint);
}

valarray<double>
cfl::atOrigin (const Slice &rSlice)
{
  const vector<unsigned> &rIx = rSlice.dependence ();

  if (rIx.size () == 0)
    {
      return rSlice.values ();
    }

  PRECONDITION (rIx.size () > 0);
  const IModel &rModel = rSlice.model ();

  valarray<size_t> uIx (rIx.size ());
  copy (rIx.begin (), rIx.end (), begin (uIx));
  valarray<double> uPoint (rModel.origin ()[uIx]);

  ASSERT (uPoint.size () == rIx.size ());

  return interpolate (rSlice) (uPoint);
}
