// do not include this file

template <class InIt1, class InIt2>
void
cfl::Interp::assign (InIt1 itArgBegin, InIt1 itArgEnd, InIt2 itValBegin)
{
  std::vector<double> uArg (itArgBegin, itArgEnd);
  std::vector<double> uVal (itValBegin, itValBegin + (itArgEnd - itArgBegin));
  return m_uP.reset (m_uP->newObject (uArg, uVal));
}

inline cfl::Function
cfl::Interp::interp () const
{
  return m_uP->interp ();
}

inline cfl::Function
cfl::Interp::deriv () const
{
  return m_uP->deriv ();
}

inline cfl::Function
cfl::Interp::deriv2 () const
{
  return m_uP->deriv2 ();
}
