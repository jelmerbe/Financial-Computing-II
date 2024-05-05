// do not include this

inline double
cfl::Function::operator() (double dX) const
{
  PRECONDITION (belongs (dX));

  return m_pF->operator() (dX);
}

inline bool
cfl::Function::belongs (double dX) const
{
  return m_pF->belongs (dX);
}

inline cfl::Function
cfl::operator- (const cfl::Function &rF)
{
  return apply (rF, [] (double dX) { return -dX; });
}

inline cfl::Function
cfl::abs (const cfl::Function &rF)
{
  return apply (rF, [] (double dX) { return std::abs (dX); });
}

inline cfl::Function
cfl::exp (const cfl::Function &rF)
{
  return apply (rF, [] (double dX) { return std::exp (dX); });
}

inline cfl::Function
cfl::log (const cfl::Function &rF)
{
  return apply (rF, [] (double dX) { return std::log (dX); });
}

inline cfl::Function
cfl::sqrt (const cfl::Function &rF)
{
  return apply (rF, [] (double dX) { return std::sqrt (dX); });
}

inline cfl::Function
cfl::operator* (const cfl::Function &rF, const cfl::Function &rG)
{
  return apply (rF, rG, std::multiplies<double> ());
}

inline cfl::Function
cfl::operator* (double dX, const cfl::Function &rF)
{
  return apply (rF, [dX] (double dY) { return dX * dY; });
}

inline cfl::Function
cfl::operator* (const cfl::Function &rF, double dX)
{
  return dX * rF;
}

inline cfl::Function
cfl::operator+ (const cfl::Function &rF, const cfl::Function &rG)
{
  return apply (rF, rG, std::plus<double> ());
}

inline cfl::Function
cfl::operator+ (double dX, const cfl::Function &rF)
{
  return apply (rF, [dX] (double dY) { return dX + dY; });
}

inline cfl::Function
cfl::operator+ (const cfl::Function &rF, double dX)
{
  return dX + rF;
}

inline cfl::Function
cfl::operator- (const cfl::Function &rF, const cfl::Function &rG)
{
  return apply (rF, rG, std::minus<double> ());
}

inline cfl::Function
cfl::operator- (double dX, const cfl::Function &rF)
{
  return apply (rF, [dX] (double dY) { return dX - dY; });
}

inline cfl::Function
cfl::operator- (const cfl::Function &rF, double dX)
{
  return apply (rF, [dX] (double dY) { return dY - dX; });
}

inline cfl::Function
cfl::operator/ (const cfl::Function &rF, const cfl::Function &rG)
{
  return apply (rF, rG, std::divides<double> ());
}

inline cfl::Function
cfl::operator/ (double dX, const cfl::Function &rF)
{
  return apply (rF, [dX] (double dY) { return dX / dY; });
}

inline cfl::Function
cfl::operator/ (const cfl::Function &rF, double dX)
{
  return apply (rF, [dX] (double dY) { return dY / dX; });
}

inline cfl::Function
cfl::pow (const cfl::Function &rF, double dX)
{
  return apply (rF, [dX] (double dY) { return std::pow (dY, dX); });
}
