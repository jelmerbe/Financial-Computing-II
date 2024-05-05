#include "cfl/RootD.hpp"
#include "cfl/Error.hpp"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>

using namespace cfl;
using namespace std;

// class RootD

cfl::RootD::RootD (IRootD *pNewP) : m_uP (pNewP) {}

// Polishing solver from GSL

namespace cflRootD
{
double
value (double dX, void *pF)
{
  const std::vector<Function> &rF = *(const std::vector<Function> *)pF;

  ASSERT (rF.front ().belongs (dX));

  return rF.front () (dX);
}

double
deriv (double dX, void *pF)
{
  const std::vector<Function> &rF = *(const std::vector<Function> *)pF;

  ASSERT (rF.back ().belongs (dX));

  return rF.back () (dX);
}

void
valueDeriv (double dX, void *pF, double *pV, double *pD)
{
  const std::vector<Function> &rF = *(const std::vector<Function> *)pF;

  ASSERT (rF.front ().belongs (dX) && rF.back ().belongs (dX));

  *pV = rF.front () (dX);
  *pD = rF.back () (dX);
}

class GSL_FDFSolver : public IRootD
{
public:
  GSL_FDFSolver (double dAbsErr, double dRelErr, unsigned iMaxSteps,
                 const gsl_root_fdfsolver_type *pT)
  {
    m_dAbsErr = dAbsErr;
    m_dRelErr = dRelErr;
    m_iMaxSteps = iMaxSteps;
    m_bFErr = false;
    m_pT = pT;
  }

  GSL_FDFSolver (double dFErr, unsigned iMaxSteps,
                 const gsl_root_fdfsolver_type *pT)
  {
    m_dFErr = dFErr;
    m_iMaxSteps = iMaxSteps;
    m_bFErr = true;
    m_pT = pT;
  }

  double
  find (const cfl::Function &rF, const cfl::Function &rDF, double dX0) const
  {
    gsl_function_fdf uF;
    std::vector<Function> uG = { rF, rDF };

    uF.f = &value;
    uF.df = &deriv;
    uF.fdf = &valueDeriv;
    uF.params = &uG;

    std::unique_ptr<gsl_root_fdfsolver, decltype (&gsl_root_fdfsolver_free)>
        uSolver (gsl_root_fdfsolver_alloc (m_pT), &gsl_root_fdfsolver_free);

    gsl_root_fdfsolver_set (uSolver.get (), &uF, dX0);
    int iStatus = GSL_CONTINUE;
    double dX = dX0;
    unsigned iSteps = 0;

    if (m_bFErr)
      {
        double dY;
        while ((iStatus == GSL_CONTINUE) && (iSteps < m_iMaxSteps))
          {
            iSteps++;
            gsl_root_fdfsolver_iterate (uSolver.get ());
            dX = gsl_root_fdfsolver_root (uSolver.get ());
            dY = GSL_FN_FDF_EVAL_F (&uF, dX);
            iStatus = gsl_root_test_residual (dY, m_dFErr);
          }
      }
    else
      {
        ASSERT (m_bFErr == false);

        while ((iStatus == GSL_CONTINUE) && (iSteps < m_iMaxSteps))
          {
            iSteps++;
            gsl_root_fdfsolver_iterate (uSolver.get ());
            dX = gsl_root_fdfsolver_root (uSolver.get ());
            iStatus = gsl_root_test_delta (dX, dX0, m_dAbsErr, m_dRelErr);
            dX0 = dX;
          }
      }

    ASSERT ((iStatus == GSL_SUCCESS) || (iSteps == m_iMaxSteps));

    return dX;
  }

private:
  double m_dAbsErr, m_dRelErr, m_dFErr;
  unsigned m_iMaxSteps;
  bool m_bFErr;
  const gsl_root_fdfsolver_type *m_pT;
};
}

using namespace cflRootD;

// functions from NRootD

RootD
cfl::NRootD::newton (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return RootD (new GSL_FDFSolver (dAbsErr, dRelErr, iMaxSteps,
                                   gsl_root_fdfsolver_newton));
}

RootD
cfl::NRootD::newton (double dFErr, unsigned iMaxSteps)
{
  return RootD (
      new GSL_FDFSolver (dFErr, iMaxSteps, gsl_root_fdfsolver_newton));
}

RootD
cfl::NRootD::secant (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return RootD (new GSL_FDFSolver (dAbsErr, dRelErr, iMaxSteps,
                                   gsl_root_fdfsolver_secant));
}

RootD
cfl::NRootD::secant (double dFErr, unsigned iMaxSteps)
{
  return RootD (
      new GSL_FDFSolver (dFErr, iMaxSteps, gsl_root_fdfsolver_secant));
}

RootD
cfl::NRootD::steffenson (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return RootD (new GSL_FDFSolver (dAbsErr, dRelErr, iMaxSteps,
                                   gsl_root_fdfsolver_steffenson));
}

RootD
cfl::NRootD::steffenson (double dFErr, unsigned iMaxSteps)
{
  return RootD (
      new GSL_FDFSolver (dFErr, iMaxSteps, gsl_root_fdfsolver_steffenson));
}
