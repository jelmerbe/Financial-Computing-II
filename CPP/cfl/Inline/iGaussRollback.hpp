// do not include this file

inline void
cfl::GaussRollback::rollback (std::valarray<double> &rValues) const
{
  PRECONDITION (rValues.size () == m_iSize);

  m_uP->rollback (rValues);
}
