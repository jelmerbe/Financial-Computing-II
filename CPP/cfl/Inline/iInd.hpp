// do not include this file directly

inline void
cfl::Ind::indicator (std::valarray<double> &rValues, double dBarrier) const
{
  return m_pInd->indicator (rValues, dBarrier);
}
