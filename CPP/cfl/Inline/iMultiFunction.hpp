// do not include this file

// inline member functions

inline std::valarray<double>
cfl::MultiFunction::operator() (const std::valarray<double> &rX) const
{
  PRECONDITION (belongs (rX));

  return (*m_pF) (rX);
}

inline std::valarray<double>
cfl::MultiFunction::operator() (const std::valarray<double> &rX,
                                const std::valarray<std::size_t> &rIx) const
{
  PRECONDITION (belongs (rX));
  PRECONDITION (rIx.size () > 0);
  PRECONDITION (rIx.size () <= dimR ());
  PRECONDITION (rIx[rIx.size () - 1] < dimR ());
  PRECONDITION (std::is_sorted (std::begin (rIx), std::end (rIx),
                                std::less_equal<std::size_t> ()));

  return (*m_pF) (rX, rIx);
}

inline bool
cfl::MultiFunction::belongs (const std::valarray<double> &rX) const
{
  PRECONDITION (dimD () > 0);
  PRECONDITION (dimR () > 0);
  PRECONDITION (rX.size () == dimD ());

  return m_pF->belongs (rX);
}

inline unsigned
cfl::MultiFunction::dimD () const
{
  return m_pF->dimD ();
}

inline unsigned
cfl::MultiFunction::dimR () const
{
  return m_pF->dimR ();
}

// global inline functions

inline cfl::MultiFunction
cfl::operator- (const cfl::MultiFunction &rF)
{
  return apply (rF, [] (const std::valarray<double> &rX) { return -rX; });
}

inline cfl::MultiFunction
cfl::abs (const cfl::MultiFunction &rF)
{
  return apply (
      rF, [] (const std::valarray<double> &rX) { return std::abs (rX); });
}

inline cfl::MultiFunction
cfl::exp (const cfl::MultiFunction &rF)
{
  return apply (
      rF, [] (const std::valarray<double> &rX) { return std::exp (rX); });
}

inline cfl::MultiFunction
cfl::log (const cfl::MultiFunction &rF)
{
  return apply (
      rF, [] (const std::valarray<double> &rX) { return std::log (rX); });
}

inline cfl::MultiFunction
cfl::sqrt (const cfl::MultiFunction &rF)
{
  return apply (
      rF, [] (const std::valarray<double> &rX) { return std::sqrt (rX); });
}

// sum

inline cfl::MultiFunction
cfl::operator+ (const cfl::MultiFunction &rF, const cfl::MultiFunction &rG)
{
  return apply (rF, rG,
                [] (const std::valarray<double> &rX,
                    const std::valarray<double> &rY) { return rX + rY; });
}

inline cfl::MultiFunction
cfl::operator+ (const std::valarray<double> &rX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rX + rY; });
}

inline cfl::MultiFunction
cfl::operator+ (const cfl::MultiFunction &rF, const std::valarray<double> &rX)
{
  return rX + rF;
}

inline cfl::MultiFunction
cfl::operator+ (double dX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return dX + rY; });
}

inline cfl::MultiFunction
cfl::operator+ (const cfl::MultiFunction &rF, double dX)
{
  return dX + rF;
}

// product

inline cfl::MultiFunction
cfl::operator* (const cfl::MultiFunction &rF, const cfl::MultiFunction &rG)
{
  return apply (rF, rG,
                [] (const std::valarray<double> &rX,
                    const std::valarray<double> &rY) { return rX * rY; });
}

inline cfl::MultiFunction
cfl::operator* (const std::valarray<double> &rX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rX * rY; });
}

inline cfl::MultiFunction
cfl::operator* (const cfl::MultiFunction &rF, const std::valarray<double> &rX)
{
  return rX * rF;
}

inline cfl::MultiFunction
cfl::operator* (double dX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return dX * rY; });
}

inline cfl::MultiFunction
cfl::operator* (const cfl::MultiFunction &rF, double dX)
{
  return dX * rF;
}

// difference

inline cfl::MultiFunction
cfl::operator- (const cfl::MultiFunction &rF, const cfl::MultiFunction &rG)
{
  return apply (rF, rG,
                [] (const std::valarray<double> &rX,
                    const std::valarray<double> &rY) { return rX - rY; });
}

inline cfl::MultiFunction
cfl::operator- (const std::valarray<double> &rX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rX - rY; });
}

inline cfl::MultiFunction
cfl::operator- (const cfl::MultiFunction &rF, const std::valarray<double> &rX)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rY - rX; });
}

inline cfl::MultiFunction
cfl::operator- (double dX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return dX - rY; });
}

inline cfl::MultiFunction
cfl::operator- (const cfl::MultiFunction &rF, double dX)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return rY - dX; });
}

// division

inline cfl::MultiFunction
cfl::operator/ (const cfl::MultiFunction &rF, const cfl::MultiFunction &rG)
{
  return apply (rF, rG,
                [] (const std::valarray<double> &rX,
                    const std::valarray<double> &rY) { return rX / rY; });
}

inline cfl::MultiFunction
cfl::operator/ (const std::valarray<double> &rX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rX / rY; });
}

inline cfl::MultiFunction
cfl::operator/ (const cfl::MultiFunction &rF, const std::valarray<double> &rX)
{
  return apply (rF,
                [rX] (const std::valarray<double> &rY) { return rY / rX; });
}

inline cfl::MultiFunction
cfl::operator/ (double dX, const cfl::MultiFunction &rF)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return dX / rY; });
}

inline cfl::MultiFunction
cfl::operator/ (const cfl::MultiFunction &rF, double dX)
{
  return apply (rF,
                [dX] (const std::valarray<double> &rY) { return rY / dX; });
}
