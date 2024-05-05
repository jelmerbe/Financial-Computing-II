// do not include this file

inline double
cfl::RootD::find (const cfl::Function &rF, const cfl::Function &rDF,
                  double dX0) const
{
  PRECONDITION (rF.belongs (dX0) && rDF.belongs (dX0));

  return m_uP->find (rF, rDF, dX0);
}
