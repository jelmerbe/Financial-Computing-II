#include "cfl/GaussRollback.hpp"
#include "cfl/Error.hpp"
#include <functional>
#include <gsl/gsl_cblas.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_vector.h>

using namespace cfl;
using namespace std;

// class GaussRollback

cfl::GaussRollback::GaussRollback (IGaussRollback *pNewP) : m_uP (pNewP) {}

std::valarray<double>
state (unsigned iSize, double dH)
{
  std::valarray<double> uState (iSize);
  uState[0] = -((iSize - 1) * dH) / 2.;

  std::transform (begin (uState), end (uState) - 1, begin (uState) + 1,
                  [dH] (double dX) { return dX + dH; });

  return uState;
}

void
cfl::GaussRollback::rollback (std::valarray<double> &rValues,
                              std::valarray<double> &rDelta) const
{
  PRECONDITION (m_dVar > cfl::EPS);

  std::valarray<double> uState = state (m_iSize, m_dH);
  rDelta = rValues * uState;
  rollback (rValues);
  rollback (rDelta);
  rDelta -= (rValues * uState);
  rDelta /= m_dVar;
}

void
cfl::GaussRollback::rollback (std::valarray<double> &rValues,
                              std::valarray<double> &rDelta,
                              std::valarray<double> &rGamma) const
{
  PRECONDITION (m_dVar > cfl::EPS);

  std::valarray<double> uState = state (m_iSize, m_dH);
  std::valarray<double> uState2 = uState * uState;
  rDelta = rValues * uState;
  rGamma = rValues * uState2;
  rollback (rValues);
  rollback (rDelta);
  rollback (rGamma);
  rGamma += (-2. * uState * rDelta + uState2 * rValues);
  rGamma /= m_dVar;
  rGamma -= rValues;
  rGamma /= m_dVar;
  rDelta -= (rValues * uState);
  rDelta /= m_dVar;
}

void
cfl::GaussRollback::assign (unsigned iSize, double dH, double dVar)
{
  m_uP.reset (m_uP->newObject (iSize, dH, dVar));
  m_dH = dH;
  m_iSize = iSize;
  m_dVar = dVar;
}

void
cfl::GaussRollback::vega (std::valarray<double> &rGamma) const
{
  rGamma *= std::sqrt (m_dVar);
}

// NGaussRollback

namespace cflGaussRollback
{

// Explicit scheme
void
explicitStep (std::valarray<double> &rValues, std::valarray<double> &rTemp,
              double dP)
{
  PRECONDITION (rValues.size () == rTemp.size ());
  PRECONDITION (rValues.size () > 2);

  unsigned iSize = static_cast<unsigned> (rTemp.size () - 2);

  ASSERT (iSize > 0);

  rTemp = -2. * rValues;
  gsl_vector_view uTemp = gsl_vector_view_array (&rTemp[1], iSize);
  gsl_vector_const_view uV1 = gsl_vector_const_view_array (&rValues[2], iSize);
  gsl_vector_add (&uTemp.vector, &uV1.vector);

  gsl_vector_const_view uV2 = gsl_vector_const_view_array (&rValues[0], iSize);
  gsl_vector_add (&uTemp.vector, &uV2.vector);

  // second derivatives at boundary points equal to neighbors
  rTemp[0] = rTemp[1];
  rTemp[rTemp.size () - 1] = rTemp[rTemp.size () - 2];
  rTemp *= dP;
  rValues += rTemp;
}

// class Explicit
class Explicit : public IGaussRollback
{
public:
  Explicit (double dP, unsigned iSize = 0, double dH = 0, double dVar = 0)
      : m_dP (dP), m_dH (dH), m_dVar (dVar), m_dQ (0.), m_iSize (iSize),
        m_iSteps (0)
  {
    bool bB = (m_dP > 0) && (dP <= 0.5);

    PRECONDITION (bB);

    if (!bB)
      {
        throw (cfl::NError::range ("step of explicit scheme"));
      }
    if (m_iSize >= 3)
      {
        ASSERT ((m_dVar > 0) && (m_dH > 0));

        double dX = 2 * m_dH * m_dH;
        m_iSteps = static_cast<unsigned> (std::ceil (m_dVar / (dX * m_dP)));
        m_dQ = min (m_dVar / (dX * m_iSteps), m_dP);

        POSTCONDITION ((m_dQ <= m_dP) && (m_dQ > 0));
      }
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new Explicit (m_dP, iSize, dH, dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    PRECONDITION (rValues.size () == m_iSize);
    PRECONDITION ((m_dQ > 0) && (m_dQ <= 0.5) && (m_iSteps > 0));

    if (m_iSize >= 3)
      {
        std::valarray<double> uTemp (m_iSize);
        for (unsigned i = 0; i < m_iSteps; i++)
          {
            explicitStep (rValues, uTemp, m_dQ);
          }
      }
  }

private:
  double m_dP, m_dH, m_dVar, m_dQ;
  unsigned m_iSize, m_iSteps;
};

// class Theta
class Theta : public IGaussRollback
{
public:
  Theta (double dTheta, const std::function<double (double)> &rP,
         unsigned iSize = 0, double dH = 0., double dVar = 0.)
      : m_dTheta (dTheta), m_dH (dH), m_dVar (dVar), m_dQ (0.), m_uP (rP),
        m_iSize (iSize), m_iSteps (0)
  {
    if (m_iSize >= 2)
      {
        double dP = rP (dH);

        PRECONDITION ((dP > 0) && (m_dTheta > 0) && (m_dTheta <= 1));
        PRECONDITION ((m_dVar > 0) && (m_dH > 0));

        double dX = 2 * m_dH * m_dH;
        m_iSteps = static_cast<unsigned> (std::ceil (m_dVar / (dX * dP)));
        m_dQ = min (dP, m_dVar / (dX * m_iSteps));
        m_uDiag.resize (m_iSize);
        m_uDiag = 1. + 2. * m_dQ * m_dTheta;

        // first m_iSize-1 elements for upper diagonal
        // last m_iSize-1 elements for lower diagonal
        m_uU.resize (m_iSize);
        m_uU = -m_dQ * m_dTheta;

        // boundary condition: second derivative is zero
        m_uDiag[0] = 1.;
        m_uDiag[m_uDiag.size () - 1] = 1.;
        m_uU[0] = 0.;
        m_uU[m_uU.size () - 1] = 0.;
      }
  }

  Theta (double dTheta, double dP, unsigned iSize = 0, double dH = 0.,
         double dVar = 0.)
      : Theta (
          dTheta, [dP] (double) { return dP; }, iSize, dH, dVar)
  {
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new Theta (m_dTheta, m_uP, iSize, dH, dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    PRECONDITION (rValues.size () == m_iSize);

    if ((m_iSize >= 2) && (m_iSteps > 0))
      {
        gsl_vector_const_view uDiag
            = gsl_vector_const_view_array (&m_uDiag[0], m_uDiag.size ());
        gsl_vector_const_view uU
            = gsl_vector_const_view_array (&m_uU[0], m_uU.size () - 1);
        gsl_vector_const_view uL
            = gsl_vector_const_view_array (&m_uU[1], m_uU.size () - 1);
        gsl_vector_view uV
            = gsl_vector_view_array (&rValues[0], rValues.size ());

        std::valarray<double> uTemp (m_iSize);
        gsl_vector_view uC = gsl_vector_view_array (&uTemp[0], m_iSize);

        for (unsigned i = 0; i < m_iSteps; i++)
          {
            if ((m_iSize >= 3) && (m_dTheta < 1))
              {
                explicitStep (rValues, uTemp, m_dQ * (1. - m_dTheta));
              }
            uTemp = rValues;
            gsl_linalg_solve_tridiag (&uDiag.vector, &uU.vector, &uL.vector,
                                      &uC.vector, &uV.vector);
          }
      }
  }

private:
  double m_dTheta, m_dH, m_dVar, m_dQ;
  std::function<double (double)> m_uP;
  unsigned m_iSize, m_iSteps;
  std::valarray<double> m_uDiag, m_uU;
};

// Fast Fourier Transform

// Radix-2
void
weights2 (unsigned iSize, double dH, double dVar, std::valarray<double> &rW)
{
  PRECONDITION (rW.size () == iSize);
  PRECONDITION ((iSize > 0) && (dH > 0) && (dVar > 0));

  const double dA = 2 * dVar * std::pow (M_PI / (iSize * dH), 2);
  rW[0] = 1.;
  unsigned i;
  double dX;
  for (i = 1; 2 * i < iSize; i++)
    {
      dX = std::exp (-(i * i * dA));
      rW[i] = dX;
      rW[iSize - i] = dX;
    }
  if (2 * i == iSize)
    {
      rW[i] = std::exp (-(i * i * dA));
    }
}

class FFT2 : public IGaussRollback
{
public:
  FFT2 () {}

  FFT2 (unsigned iSize, double dH, double dVar)
      : m_iSize (iSize), m_dH (dH), m_dVar (dVar), m_uW (iSize)
  {
    ASSERT ((m_iSize > 0) && (m_dH > 0) && (m_dVar > 0));
    ASSERT (std::log2 (m_iSize) == std::round (std::log2 (m_iSize)));

    weights2 (m_iSize, m_dH, m_dVar, m_uW);
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new FFT2 (iSize, dH, dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    PRECONDITION ((m_dH > 0) && (m_dVar > 0));
    PRECONDITION (rValues.size () == m_iSize);

    gsl_fft_real_radix2_transform (begin (rValues), 1, rValues.size ());
    rValues *= m_uW;
    gsl_fft_halfcomplex_radix2_inverse (begin (rValues), 1, rValues.size ());
  }

private:
  unsigned m_iSize;
  double m_dH, m_dVar;
  std::valarray<double> m_uW;
};

// General FFT
void
weights (unsigned iSize, double dH, double dVar, std::valarray<double> &rW)
{
  PRECONDITION (rW.size () == iSize);
  PRECONDITION ((iSize > 0) && (dH > 0) && (dVar > 0));

  const double dA = 2 * dVar * std::pow (M_PI / (iSize * dH), 2);
  rW[0] = 1.;
  unsigned i;
  double dX;
  for (i = 1; 2 * i < iSize; i++)
    {
      dX = std::exp (-(i * i * dA));
      rW[2 * i - 1] = dX;
      rW[2 * i] = dX;
    }
  if (2 * i == iSize)
    {
      rW[2 * i - 1] = std::exp (-(i * i * dA));
    }
}

class FFT : public IGaussRollback
{
public:
  FFT () {}

  FFT (unsigned iSize, double dH, double dVar)
      : m_iSize (iSize), m_dH (dH), m_dVar (dVar), m_uW (iSize),
        m_pRTable (gsl_fft_real_wavetable_alloc (iSize),
                   &gsl_fft_real_wavetable_free),
        m_pImTable (gsl_fft_halfcomplex_wavetable_alloc (iSize),
                    &gsl_fft_halfcomplex_wavetable_free),
        m_pWork (gsl_fft_real_workspace_alloc (iSize),
                 &gsl_fft_real_workspace_free)
  {
    ASSERT ((m_iSize > 0) && (m_dH > 0) && (m_dVar > 0));

    weights (m_iSize, m_dH, m_dVar, m_uW);
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new FFT (iSize, dH, dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    PRECONDITION ((m_dH > 0) && (m_dVar > 0));
    PRECONDITION (rValues.size () == m_iSize);

    gsl_fft_real_transform (begin (rValues), 1, rValues.size (),
                            m_pRTable.get (), m_pWork.get ());
    rValues *= m_uW;
    gsl_fft_halfcomplex_inverse (begin (rValues), 1, rValues.size (),
                                 m_pImTable.get (), m_pWork.get ());
  }

private:
  unsigned m_iSize;
  double m_dH, m_dVar;
  std::valarray<double> m_uW;
  std::shared_ptr<gsl_fft_real_wavetable> m_pRTable;
  std::shared_ptr<gsl_fft_halfcomplex_wavetable> m_pImTable;
  std::shared_ptr<gsl_fft_real_workspace> m_pWork;
};

// class Chain
class Chain : public IGaussRollback
{
public:
  Chain (unsigned iExpl, const cfl::GaussRollback &rMain, unsigned iImpl,
         double dExplP, double dImplP, unsigned iSize = 0, double dH = 0.,
         double dVar = 0.)
      : m_iExpl (iExpl), m_iImpl (iImpl), m_dExplP (dExplP), m_dImplP (dImplP),
        m_uMain (rMain), m_uExpl (cfl::NGaussRollback::expl (dExplP)),
        m_uImpl (cfl::NGaussRollback::impl (dImplP))
  {
    double dExplVar = 2. * dH * dH * m_dExplP * m_iExpl;
    double dImplVar = 2. * dH * dH * m_dImplP * m_iImpl;
    double dV = dVar - (dExplVar + dImplVar);
    if (dV > 0)
      {
        m_bMain = true;
        m_uMain.assign (iSize, dH, dV);
        m_uExpl.assign (iSize, dH, dExplVar);
        m_uImpl.assign (iSize, dH, dImplVar);
      }
    else // we only do explicit
      {
        m_bMain = false;
        m_uExpl.assign (iSize, dH, dVar);
      }
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new Chain (m_iExpl, m_uMain, m_iImpl, m_dExplP, m_dImplP, iSize, dH,
                      dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    if ((m_iExpl > 0) || (!m_bMain))
      {
        m_uExpl.rollback (rValues);
      }
    if (m_bMain)
      {
        m_uMain.rollback (rValues);
        m_uImpl.rollback (rValues);
      }
  }

private:
  unsigned m_iExpl, m_iImpl;
  double m_dExplP, m_dImplP;
  GaussRollback m_uMain, m_uExpl, m_uImpl;
  bool m_bMain;
};

using namespace cfl::NGaussRollback;

// class Default
class DefaultChain : public IGaussRollback
{
public:
  DefaultChain (const std::string &sFast, unsigned iSize = 0, double dH = 0,
                double dVar = 0)
      : m_sFast (sFast)
  {
    PRECONDITION ((sFast == "crankNicolson") || (sFast == "fft2")
                  || (sFast == "fft"));

    if (iSize > 0)
      {
        ASSERT ((dVar > 0) && (dH > 0));

        unsigned iExpl, iImpl;
        if (m_sFast == "crankNicolson")
          {
            iExpl = 2 * (std::ceil (dVar / dH) + 1);
            iImpl = iExpl / 2;
            m_uRollback = chain (iExpl, crankNicolson (), iImpl);
          }
        // FFT
        iExpl = 2 * std::ceil (std::log2 (iSize)) + 10;
        iImpl = iExpl / 2;
        if (m_sFast == "fft2")
          {
            m_uRollback = chain (iExpl, fft2 (), iImpl);
          }
        if (m_sFast == "fft")
          {
            m_uRollback = chain (iExpl, fft (), iImpl);
          }
        m_uRollback.assign (iSize, dH, dVar);
      }
  }

  IGaussRollback *
  newObject (unsigned iSize, double dH, double dVar) const
  {
    return new DefaultChain (m_sFast, iSize, dH, dVar);
  }

  void
  rollback (std::valarray<double> &rValues) const
  {
    m_uRollback.rollback (rValues);
  }

private:
  std::string m_sFast;
  GaussRollback m_uRollback;
}; // namespace cflGaussRollback
} // namespace cflGaussRollback

cfl::GaussRollback
cfl::NGaussRollback::expl (double dP)
{
  PRECONDITION ((dP > 0) && (dP <= 0.5));

  return GaussRollback (new cflGaussRollback::Explicit (dP));
}

cfl::GaussRollback
cfl::NGaussRollback::impl (double dP)
{
  PRECONDITION (dP > 0);

  double dTheta = 1;
  return GaussRollback (new cflGaussRollback::Theta (dTheta, dP));
}

cfl::GaussRollback
cfl::NGaussRollback::crankNicolson (double dR)
{
  PRECONDITION (dR > 0);

  double dTheta = 0.5;
  std::function<double (double)> uP
      = [dR] (double dH) { return dR / (2. * dH); };

  return GaussRollback (new cflGaussRollback::Theta (dTheta, uP));
}

cfl::GaussRollback
cfl::NGaussRollback::fft2 ()
{
  return GaussRollback (new cflGaussRollback::FFT2 ());
}

cfl::GaussRollback
cfl::NGaussRollback::fft ()
{
  return GaussRollback (new cflGaussRollback::FFT ());
}

cfl::GaussRollback
cfl::NGaussRollback::chain (unsigned iExplSteps, const GaussRollback &rMain,
                            unsigned iImplSteps, double dExplP, double dImplP)
{
  return GaussRollback (new cflGaussRollback::Chain (
      iExplSteps, rMain, iImplSteps, dExplP, dImplP));
}

cfl::GaussRollback
cfl::NGaussRollback::chain (const char *sFast)
{
  return GaussRollback (new cflGaussRollback::DefaultChain (sFast));
}
