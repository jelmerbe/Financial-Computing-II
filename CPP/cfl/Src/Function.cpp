#include "cfl/Function.hpp"
#include "cfl/Error.hpp"
#include <cmath>

using namespace cfl;
using namespace std;

cfl::Function::Function (IFunction *pNewP) : m_pF (pNewP) {}

namespace cflFunction
{
//  CLASS: Adapter

class Adapter : public cfl::IFunction
{
public:
  Adapter (const function<double (double)> &rF,
           const function<bool (double)> &rB)
      : m_uF (rF), m_uB (rB)
  {
  }

  Adapter (const function<double (double)> &rF, double dL, double dR)
      : Adapter (rF, [dL, dR] (double dX) { return (dL <= dX) && (dX <= dR); })
  {
    POSTCONDITION (dL <= dR);
  }

  Adapter (double dV, double dL = -OMEGA, double dR = OMEGA)
      : Adapter ([dV] (double dX) { return dV; }, dL, dR)
  {
  }

  double
  operator() (double dX) const
  {
    PRECONDITION (belongs (dX));

    return m_uF (dX);
  }
  bool
  belongs (double dX) const
  {
    return m_uB (dX);
  }

private:
  function<double (double)> m_uF;
  function<bool (double)> m_uB;
};

// CLASS: Composite

class Composite : public IFunction
{
public:
  Composite (const Function &rF, const function<double (double)> &rOp)
      : m_uF (rF), m_uOp (rOp)
  {
  }

  double
  operator() (double dX) const
  {
    return m_uOp (m_uF (dX));
  }
  bool
  belongs (double dX) const
  {
    return m_uF.belongs (dX);
  }

private:
  Function m_uF;
  function<double (double)> m_uOp;
};

// CLASS: BinComposite

class BinComposite : public IFunction
{
public:
  BinComposite (const Function &rF1, const Function &rF2,
                const function<double (double, double)> &rOp)
      : m_uF1 (rF1), m_uF2 (rF2), m_uOp (rOp)
  {
  }

  double
  operator() (double dX) const
  {
    return m_uOp (m_uF1 (dX), m_uF2 (dX));
  }
  bool
  belongs (double dX) const
  {
    return (m_uF1.belongs (dX)) && (m_uF2.belongs (dX));
  }

private:
  Function m_uF1;
  Function m_uF2;
  function<double (double, double)> m_uOp;
};
} // namespace cflFunction

// CLASS: Function

cfl::Function::Function (double dV, double dL, double dR)
    : m_pF (new cflFunction::Adapter (dV, dL, dR))
{
}

cfl::Function::Function (const function<double (double)> &rF, double dL,
                         double dR)
    : m_pF (new cflFunction::Adapter (rF, dL, dR))
{
}

cfl::Function::Function (const function<double (double)> &rF,
                         const function<bool (double)> &rBelongs)
    : m_pF (new cflFunction::Adapter (rF, rBelongs))
{
}

Function &
cfl::Function::operator= (double dV)
{
  m_pF.reset (new cflFunction::Adapter (dV));
  return *this;
}

Function &
cfl::Function::operator+= (const Function &rF)
{
  m_pF.reset (new cflFunction::BinComposite (*this, rF, plus<double> ()));
  return *this;
}

Function &
cfl::Function::operator*= (const Function &rF)
{
  m_pF.reset (
      new cflFunction::BinComposite (*this, rF, multiplies<double> ()));
  return *this;
}

Function &
cfl::Function::operator-= (const Function &rF)
{
  m_pF.reset (new cflFunction::BinComposite (*this, rF, minus<double> ()));
  return *this;
}

Function &
cfl::Function::operator/= (const Function &rF)
{
  m_pF.reset (new cflFunction::BinComposite (*this, rF, divides<double> ()));
  return *this;
}

Function &
cfl::Function::operator+= (double dX)
{
  m_pF.reset (new cflFunction::Composite (
      *this, [dX] (double dY) { return dY + dX; }));
  return *this;
}

Function &
cfl::Function::operator-= (double dX)
{
  m_pF.reset (new cflFunction::Composite (
      *this, [dX] (double dY) { return dY - dX; }));
  return *this;
}

Function &
cfl::Function::operator*= (double dX)
{
  m_pF.reset (new cflFunction::Composite (
      *this, [dX] (double dY) { return dY * dX; }));
  return *this;
}

Function &
cfl::Function::operator/= (double dX)
{
  m_pF.reset (new cflFunction::Composite (
      *this, [dX] (double dY) { return dY / dX; }));
  return *this;
}

// GLOBAL

cfl::Function
cfl::apply (const cfl::Function &rF, const function<double (double)> &rOp)
{
  return Function (new cflFunction::Composite (rF, rOp));
}

cfl::Function
cfl::apply (const cfl::Function &rF, const cfl::Function &rG,
            const function<double (double, double)> &rOp)
{
  return Function (new cflFunction::BinComposite (rF, rG, rOp));
}
