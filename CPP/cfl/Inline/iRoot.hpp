// do not include this file

inline double
cfl::Root::find (const cfl::Function &rF, double dL, double dR) const
{
  PRECONDITION (dL < dR);
  PRECONDITION (rF (dL) * rF (dR) <= 0);

  return m_uP->find (rF, dL, dR);
}
