#include "cfl/MultiFunction.hpp"
#include "cfl/Error.hpp"
#include "cfl/Function.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

using namespace cfl;
using namespace std;

// main constructor
cfl::MultiFunction::MultiFunction (IMultiFunction *pNewF) : m_pF (pNewF) {}

namespace cflMultiFunction
{
// CLASS: Adapter
class Adapter : public cfl::IMultiFunction
{
public:
  Adapter (const function<valarray<double> (const valarray<double> &,
                                            const valarray<size_t> &)> &rFF,
           const function<valarray<double> (const valarray<double> &)> &rF,
           const function<bool (const valarray<double> &)> &rB, unsigned iDimD,
           unsigned iDimR)
      : m_uFF (rFF), m_uF (rF), m_uB (rB), m_iDimD (iDimD), m_iDimR (iDimR)
  {
    POSTCONDITION (m_iDimD > 0);
    POSTCONDITION (m_iDimR > 0);
  }

  Adapter (const function<valarray<double> (const valarray<double> &,
                                            const valarray<size_t> &)> &rFF,
           const function<valarray<double> (const valarray<double> &)> &rF,
           unsigned iDimD, unsigned iDimR)
      : Adapter (
          rFF, rF, [] (const valarray<double> &rX) { return true; }, iDimD,
          iDimR)
  {
  }

  Adapter (const valarray<double> &rV, unsigned iDimD)
      : Adapter ([rV] (const valarray<double> &,
                       const valarray<size_t> &rIx) { return rV[rIx]; },
                 [rV] (const valarray<double> &) { return rV; }, iDimD,
                 rV.size ())
  {
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    return m_uF (rX);
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    return m_uFF (rX, rIx);
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return m_uB (rX);
  }
  unsigned
  dimD () const
  {
    return m_iDimD;
  }

  unsigned
  dimR () const
  {
    return m_iDimR;
  }

private:
  function<valarray<double> (const valarray<double> &,
                             const valarray<size_t> &)>
      m_uFF;

  function<valarray<double> (const valarray<double> &)> m_uF;
  function<bool (const valarray<double> &)> m_uB;
  unsigned m_iDimD, m_iDimR;
};

// CLASS: Subset
class Subset : public cfl::IMultiFunction
{
public:
  Subset (const MultiFunction &rF, const valarray<size_t> &rIx)
      : m_uF (rF), m_uIx (rIx)
  {
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    return m_uF (rX, m_uIx);
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    valarray<size_t> uIx (m_uIx[rIx]);

    return m_uF (rX, uIx);
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return m_uF.belongs (rX);
  }

  unsigned
  dimD () const
  {
    return m_uF.dimD ();
  }

  unsigned
  dimR () const
  {
    return m_uIx.size ();
  }

private:
  MultiFunction m_uF;
  valarray<size_t> m_uIx;
};

// CLASS: FromFunction
class FromFunction : public cfl::IMultiFunction
{
public:
  FromFunction (const Function &rF) : m_uF (rF) {}

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    double dV = m_uF (rX[0]);
    return valarray<double> (dV, 1);
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    return operator() (rX);
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return m_uF.belongs (rX[0]);
  }

  unsigned
  dimD () const
  {
    return 1;
  }

  unsigned
  dimR () const
  {
    return 1;
  }

private:
  Function m_uF;
};

// CLASS: Composite

class Composite : public IMultiFunction
{
public:
  Composite (const MultiFunction &rF,
             const function<valarray<double> (const valarray<double> &)> &rOp)
      : m_uF (rF), m_uOp (rOp)
  {
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    return m_uOp (m_uF (rX));
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    return m_uOp (m_uF (rX, rIx));
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return m_uF.belongs (rX);
  }

  unsigned
  dimD () const
  {
    return m_uF.dimD ();
  }

  unsigned
  dimR () const
  {
    return m_uF.dimR ();
  }

private:
  MultiFunction m_uF;
  function<valarray<double> (const valarray<double> &)> m_uOp;
};

// CLASS: BinComposite

class BinComposite : public IMultiFunction
{
public:
  BinComposite (const MultiFunction &rF1, const MultiFunction &rF2,
                const function<valarray<double> (
                    const valarray<double> &, const valarray<double> &)> &rOp)
      : m_uF1 (rF1), m_uF2 (rF2), m_uOp (rOp)
  {
    POSTCONDITION (m_uF1.dimD () == m_uF2.dimD ());
    POSTCONDITION (m_uF1.dimR () == m_uF2.dimR ());
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    return m_uOp (m_uF1 (rX), m_uF2 (rX));
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    return m_uOp (m_uF1 (rX, rIx), m_uF2 (rX, rIx));
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return (m_uF1.belongs (rX)) && (m_uF2.belongs (rX));
  }

  unsigned
  dimD () const
  {
    return m_uF1.dimD ();
  }

  unsigned
  dimR () const
  {
    return m_uF1.dimR ();
  }

private:
  MultiFunction m_uF1;
  MultiFunction m_uF2;
  function<valarray<double> (const valarray<double> &,
                             const valarray<double> &)>
      m_uOp;
};
} // namespace cflMultiFunction

// CLASS: MultiFunction

// constructors

cfl::MultiFunction::MultiFunction (
    const function<valarray<double> (const valarray<double> &,
                                     const valarray<size_t> &)> &rFF,
    const function<valarray<double> (const valarray<double> &)> &rF,
    const function<bool (const valarray<double> &)> &rB, unsigned iDimD,
    unsigned iDimR)
    : m_pF (new cflMultiFunction::Adapter (rFF, rF, rB, iDimD, iDimR))
{
}

cfl::MultiFunction::MultiFunction (
    const function<valarray<double> (const valarray<double> &,
                                     const valarray<size_t> &)> &rFF,
    const function<valarray<double> (const valarray<double> &)> &rF,
    unsigned iDimD, unsigned iDimR)
    : m_pF (new cflMultiFunction::Adapter (rFF, rF, iDimD, iDimR))
{
}

cfl::MultiFunction::MultiFunction (const valarray<double> &rV, unsigned iDimD)
    : m_pF (new cflMultiFunction::Adapter (rV, iDimD))
{
}

cfl::MultiFunction::MultiFunction (const MultiFunction &rF,
                                   const valarray<size_t> &rIx)
    : m_pF (new cflMultiFunction::Subset (rF, rIx))
{
}

cfl::MultiFunction::MultiFunction (const Function &rF)
    : m_pF (new cflMultiFunction::FromFunction (rF))
{
}

// operators with multifunction

MultiFunction &
cfl::MultiFunction::operator+= (const MultiFunction &rF)
{
  m_pF.reset (new cflMultiFunction::BinComposite (*this, rF,
                                                  plus<valarray<double> > ()));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator*= (const MultiFunction &rF)
{
  m_pF.reset (new cflMultiFunction::BinComposite (
      *this, rF, multiplies<valarray<double> > ()));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator-= (const MultiFunction &rF)
{
  m_pF.reset (new cflMultiFunction::BinComposite (
      *this, rF, minus<valarray<double> > ()));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator/= (const MultiFunction &rF)
{
  m_pF.reset (new cflMultiFunction::BinComposite (
      *this, rF, divides<valarray<double> > ()));
  return *this;
}

// operators with valarray

MultiFunction &
cfl::MultiFunction::operator+= (const valarray<double> &rV)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [rV] (const valarray<double> &rY) { return rY + rV; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator-= (const valarray<double> &rV)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [rV] (const valarray<double> &rY) { return rY - rV; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator*= (const valarray<double> &rV)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [rV] (const valarray<double> &rY) { return rY * rV; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator/= (const valarray<double> &rV)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [rV] (const valarray<double> &rY) { return rY / rV; }));
  return *this;
}

// operators with double

MultiFunction &
cfl::MultiFunction::operator+= (double dX)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [dX] (const valarray<double> &rY) { return rY + dX; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator-= (double dX)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [dX] (const valarray<double> &rY) { return rY - dX; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator*= (double dX)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [dX] (const valarray<double> &rY) { return rY * dX; }));
  return *this;
}

MultiFunction &
cfl::MultiFunction::operator/= (double dX)
{
  m_pF.reset (new cflMultiFunction::Composite (
      *this, [dX] (const valarray<double> &rY) { return rY / dX; }));
  return *this;
}

// GLOBAL FUNCTIONS

// generic composition functions

cfl::MultiFunction
cfl::apply (const cfl::MultiFunction &rF,
            const function<valarray<double> (const valarray<double> &)> &rOp)
{
  return MultiFunction (new cflMultiFunction::Composite (rF, rOp));
}

cfl::MultiFunction
cfl::apply (const cfl::MultiFunction &rF, const cfl::MultiFunction &rG,
            const function<valarray<double> (const valarray<double> &,
                                             const valarray<double> &)> &rOp)
{
  return MultiFunction (new cflMultiFunction::BinComposite (rF, rG, rOp));
}

// section

namespace cflMultiFunction
{
class Section : public IMultiFunction
{
public:
  Section (const MultiFunction &rF,
           const function<valarray<double> (const valarray<double> &)> rS,
           const function<bool (const valarray<double> &)> rB, unsigned iDimD)
      : m_uF (rF), m_uS (rS), m_uB (rB), m_iDimD (iDimD)
  {
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    return m_uF (m_uS (rX));
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    return m_uF (m_uS (rX), rIx);
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    return m_uB (rX) && m_uF.belongs (m_uS (rX));
  }

  unsigned
  dimD () const
  {
    return m_iDimD;
  }

  unsigned
  dimR () const
  {
    return m_uF.dimR ();
  }

private:
  MultiFunction m_uF;
  function<valarray<double> (const valarray<double> &)> m_uS;
  function<bool (const valarray<double> &)> m_uB;
  unsigned m_iDimD;
};
} // namespace cflMultiFunction

MultiFunction
cfl::section (const MultiFunction &rF,
              const function<valarray<double> (const valarray<double> &)> rS,
              const function<bool (const valarray<double> &)> rB,
              unsigned iDimD)
{
  return MultiFunction (new cflMultiFunction::Section (rF, rS, rB, iDimD));
}

MultiFunction
cfl::section (const MultiFunction &rF, const valarray<size_t> &rFlexIx,
              const valarray<double> &rFixedArg)
{
  PRECONDITION (rF.dimD () == rFlexIx.size () + rFixedArg.size ());
  PRECONDITION (rFlexIx.size () > 0);
  PRECONDITION (rFixedArg.size () > 0);

  valarray<double> uV (rF.dimD ());

  valarray<size_t> uFullIx (rF.dimD ());
  iota (begin (uFullIx), end (uFullIx), 0);

  valarray<size_t> uFixedIx (rFixedArg.size ());
  set_difference (begin (uFullIx), end (uFullIx), begin (rFlexIx),
                  end (rFlexIx), begin (uFixedIx));

  uV[uFixedIx] = rFixedArg;

  function<valarray<double> (const valarray<double> &)> uS
      = [uV, rFlexIx] (const valarray<double> &rX) mutable {
          PRECONDITION (rX.size () == rFlexIx.size ());

          uV[rFlexIx] = rX;

          return uV;
        };

  function<bool (const valarray<double> &)> uB
      = [] (const valarray<double> &rX) { return true; };

  return cfl::section (rF, uS, uB, rFlexIx.size ());
}

// tensor

namespace cflMultiFunction
{
class Tensor : public IMultiFunction
{
public:
  Tensor (const vector<MultiFunction> &rF) : m_uF (rF)
  {
    m_iDimD = rF.front ().dimD ();

    ASSERT (all_of (rF.begin () + 1, rF.end (),
                    [iDimD = m_iDimD] (const MultiFunction &rG) {
                      return rG.dimD () == iDimD;
                    }));

    m_iDimR = accumulate (rF.begin (), rF.end (), 0,
                          [] (unsigned iDimR, const MultiFunction &rG) {
                            return iDimR + rG.dimR ();
                          });
  }

  valarray<double>
  operator() (const valarray<double> &rX) const
  {
    valarray<double> uY (dimR ());
    size_t iG = 0;
    for_each (m_uF.begin (), m_uF.end (),
              [&iG, &uY, &rX] (const MultiFunction &rG) {
                uY[slice (iG, 1, rG.dimR ())] = rG (rX);
                iG += rG.dimR ();
              });
    ASSERT (iG == dimR ());

    return uY;
  }

  valarray<double>
  operator() (const valarray<double> &rX, const valarray<size_t> &rIx) const
  {
    valarray<double> uY (rIx.size ());
    size_t iY = 0;
    size_t iF = 0;
    for_each (m_uF.begin (), m_uF.end (),
              [&iY, &iF, &uY, &rX, &rIx] (const MultiFunction &rF) {
                if ((iY < rIx.size ()) && (rIx[iY] < iF + rF.dimR ()))
                  {
                    PRECONDITION (iF <= rIx[iY]);
                    size_t iSize = lower_bound (begin (rIx) + iY, end (rIx),
                                                iF + rF.dimR ())
                                   - (begin (rIx) + iY);

                    ASSERT (iSize > 0);

                    valarray<size_t> uIx (iSize);
                    uIx = rIx[slice (iY, 1, iSize)];
                    uIx -= iF;

                    uY[slice (iY, 1, iSize)] = rF (rX, uIx);
                    iY += iSize;
                  };

                iF += rF.dimR ();
              });

    ASSERT (iY == rIx.size ());
    ASSERT (iF == dimR ());

    return uY;
  }

  bool
  belongs (const valarray<double> &rX) const
  {
    bool bB
        = all_of (m_uF.begin (), m_uF.end (),
                  [&rX] (const MultiFunction &rF) { return rF.belongs (rX); });

    return bB;
  }

  unsigned
  dimD () const
  {
    return m_iDimD;
  }

  unsigned
  dimR () const
  {
    return m_iDimR;
  }

private:
  vector<MultiFunction> m_uF;
  unsigned m_iDimD, m_iDimR;
};
} // namespace cflMultiFunction

MultiFunction
cfl::tensor (const vector<MultiFunction> &rF)
{
  return MultiFunction (new cflMultiFunction::Tensor (rF));
}
