// do not include this file

// member functions

inline cfl::Slice &
cfl::Slice::operator= (double dValue)
{
  m_uDependence.clear ();
  m_uValues.resize (1, dValue);

  return *this;
}

inline cfl::Slice &
cfl::Slice::operator+= (double dValue)
{
  m_uValues += dValue;

  return *this;
}

inline cfl::Slice &
cfl::Slice::operator-= (double dValue)
{
  m_uValues -= dValue;

  return *this;
}

inline cfl::Slice &
cfl::Slice::operator*= (double dValue)
{
  m_uValues *= dValue;

  return *this;
}

inline cfl::Slice &
cfl::Slice::operator/= (double dValue)
{
  m_uValues /= dValue;

  return *this;
}

inline cfl::Slice
cfl::Slice::apply (double f (double)) const
{
  return cfl::Slice (*m_pModel, m_iTime, m_uDependence, m_uValues.apply (f));
}

inline void
cfl::Slice::rollback (unsigned iTime)
{
  PRECONDITION (iTime <= timeIndex ());

  if (iTime < timeIndex ())
    {
      m_pModel->rollback (*this, iTime);
    }
}

inline const cfl::IModel &
cfl::Slice::model () const
{
  return *m_pModel;
}

inline unsigned
cfl::Slice::timeIndex () const
{
  return m_iTime;
}

inline const std::vector<unsigned> &
cfl::Slice::dependence () const
{
  return m_uDependence;
}

inline const std::valarray<double> &
cfl::Slice::values () const
{
  return m_uValues;
}

inline std::valarray<double> &
cfl::Slice::values ()
{
  return m_uValues;
}

inline void
cfl::Slice::assign (const cfl::IModel &rModel, unsigned iTime,
                    const std::vector<unsigned> &rDependence,
                    const std::valarray<double> &rValues)
{
  m_pModel = &rModel;
  assign (iTime, rDependence, rValues);

  POSTCONDITION (m_pModel->numberOfNodes (iTime, rDependence)
                 == rValues.size ());
}

inline void
cfl::Slice::assign (unsigned iTime, const std::vector<unsigned> &rDependence,
                    const std::valarray<double> &rValues)
{
  m_iTime = iTime;
  assign (rDependence, rValues);

  POSTCONDITION (m_pModel->numberOfNodes (iTime, rDependence)
                 == rValues.size ());
}

inline void
cfl::Slice::assign (const std::vector<unsigned> &rDependence,
                    const std::valarray<double> &rValues)
{
  if (&m_uDependence != &rDependence)
    {
      m_uDependence = rDependence;
    }
  if (&rValues != &m_uValues)
    {
      m_uValues = rValues;
    }

  POSTCONDITION (m_pModel->numberOfNodes (m_iTime, m_uDependence)
                 == rValues.size ());
}

inline void
cfl::Slice::assign (const IModel &rModel)
{
  m_pModel = &rModel;

  POSTCONDITION (m_pModel->numberOfNodes (m_iTime, m_uDependence)
                 == m_uValues.size ());
}

// Global arithmetic operators and functions.

inline cfl::Slice
cfl::operator- (const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), -rSlice.values ());
}

inline cfl::Slice
cfl::operator+ (const cfl::Slice &rSlice1, const cfl::Slice &rSlice2)
{
  cfl::Slice uSlice (rSlice1);
  uSlice += rSlice2;

  return uSlice;
}

inline cfl::Slice
cfl::operator- (const cfl::Slice &rSlice1, const cfl::Slice &rSlice2)
{
  cfl::Slice uSlice (rSlice1);
  uSlice -= rSlice2;

  return uSlice;
}

inline cfl::Slice
cfl::operator* (const cfl::Slice &rSlice1, const cfl::Slice &rSlice2)
{
  cfl::Slice uSlice (rSlice1);
  uSlice *= rSlice2;

  return uSlice;
}

inline cfl::Slice
cfl::operator/ (const cfl::Slice &rSlice1, const cfl::Slice &rSlice2)
{
  cfl::Slice uSlice (rSlice1);
  uSlice /= rSlice2;

  return uSlice;
}

inline cfl::Slice
cfl::operator+ (const cfl::Slice &rSlice, double dValue)
{
  cfl::Slice uSlice (rSlice);
  uSlice += dValue;

  return uSlice;
}

inline cfl::Slice
cfl::operator- (const cfl::Slice &rSlice, double dValue)
{
  cfl::Slice uSlice (rSlice);
  uSlice -= dValue;

  return uSlice;
}

inline cfl::Slice
cfl::operator* (const cfl::Slice &rSlice, double dValue)
{
  cfl::Slice uSlice (rSlice);
  uSlice *= dValue;

  return uSlice;
}

inline cfl::Slice
cfl::operator/ (const cfl::Slice &rSlice, double dValue)
{
  cfl::Slice uSlice (rSlice);
  uSlice /= dValue;

  return uSlice;
}

inline cfl::Slice
cfl::operator+ (double dValue, const cfl::Slice &rSlice)
{
  return rSlice + dValue;
}
inline cfl::Slice
cfl::operator- (double dValue, const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), dValue - rSlice.values ());
}

inline cfl::Slice
cfl::operator* (double dValue, const cfl::Slice &rSlice)
{
  return rSlice * dValue;
}
inline cfl::Slice
cfl::operator/ (double dValue, const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), dValue / rSlice.values ());
}

inline cfl::Slice
cfl::max (double dValue, const cfl::Slice &rSlice)
{
  return max (rSlice, dValue);
}

inline cfl::Slice
cfl::min (double dValue, const cfl::Slice &rSlice)
{
  return min (rSlice, dValue);
}

inline cfl::Slice
cfl::pow (const cfl::Slice &rSlice, double dPower)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (),
                     std::pow (rSlice.values (), dPower));
}

inline cfl::Slice
cfl::abs (const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), std::abs (rSlice.values ()));
}

inline cfl::Slice
cfl::exp (const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), std::exp (rSlice.values ()));
}

inline cfl::Slice
cfl::log (const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), std::log (rSlice.values ()));
}

inline cfl::Slice
cfl::sqrt (const cfl::Slice &rSlice)
{
  return cfl::Slice (rSlice.model (), rSlice.timeIndex (),
                     rSlice.dependence (), std::sqrt (rSlice.values ()));
}

inline cfl::Slice
cfl::indicator (const cfl::Slice &rSlice, double dBarrier)
{
  Slice uInd (rSlice);
  rSlice.model ().indicator (uInd, dBarrier);

  return uInd;
}

inline cfl::Slice
cfl::indicator (double dBarrier, const cfl::Slice &rSlice)
{
  return 1. - indicator (rSlice, dBarrier);
}

inline cfl::Slice
cfl::indicator (const cfl::Slice &rSlice, const cfl::Slice &rBarrier)
{
  return indicator (rSlice - rBarrier, 0.);
}

inline cfl::Slice
cfl::rollback (const cfl::Slice &rSlice, unsigned iTime)
{
  cfl::Slice uSlice (rSlice);
  uSlice.rollback (iTime);

  return uSlice;
}

inline cfl::MultiFunction
cfl::interpolate (const cfl::Slice &rSlice)
{
  return rSlice.model ().interpolate (rSlice);
}

inline cfl::MultiFunction
cfl::interpolate (const cfl::Slice &rSlice, unsigned iStates)
{
  std::vector<unsigned> uDepend (iStates);
  std::iota (uDepend.begin (), uDepend.end (), 0);

  return interpolate (rSlice, uDepend);
}
