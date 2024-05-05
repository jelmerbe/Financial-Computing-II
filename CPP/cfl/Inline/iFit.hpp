// do not include this file

template <class InIt1, class InIt2>
void
cfl::Fit::assign (InIt1 itArgBegin, InIt1 itArgEnd, InIt2 itValBegin)
{
  std::vector<double> uW (itArgEnd - itArgBegin, 1.);
  assign (itArgBegin, itArgEnd, itValBegin, uW.begin (), true);
}

template <class InIt1, class InIt2, class InIt3>
void
cfl::Fit::assign (InIt1 itArgBegin, InIt1 itArgEnd, InIt2 itValBegin,
                  InIt3 itWtBegin, bool bChi2)
{
  std::vector<double> uArg (itArgBegin, itArgEnd);
  std::vector<double> uVal (itValBegin, itValBegin + uArg.size ());
  std::vector<double> uWt (itWtBegin, itWtBegin + uArg.size ());
  m_uP.reset (m_uP->newObject (uArg, uVal, uWt, bChi2));
}

inline cfl::Function
cfl::Fit::fit () const
{
  return m_uP->fit ();
}

inline cfl::Function
cfl::Fit::err () const
{
  return m_uP->err ();
}

inline cfl::FitParam
cfl::Fit::param () const
{
  return m_uP->param ();
}
