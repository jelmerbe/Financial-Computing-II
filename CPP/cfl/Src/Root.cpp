#include "cfl/Root.hpp"
#include "cfl/Error.hpp"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_roots.h>

using namespace cfl;
using namespace std;

// class Root

cfl::Root::Root (IRoot *pNewP) : m_uP (pNewP) {}

// Bracketing solver from GSL

namespace cflRoot
{
double
value (double dX, void *pF)
{
  const Function &rF = *(const Function *)pF;

  ASSERT (rF.belongs (dX));

  return rF (dX);
}

class GSL_FSolver : public IRoot
{
public:
  GSL_FSolver (double dAbsErr, double dRelErr, unsigned iMaxSteps,
               const gsl_root_fsolver_type *pT)
  {
    m_dAbsErr = dAbsErr;
    m_dRelErr = dRelErr;
    m_iMaxSteps = iMaxSteps;
    m_bFErr = false;
    m_pT = pT;
  }

  GSL_FSolver (double dFErr, unsigned iMaxSteps,
               const gsl_root_fsolver_type *pT)
  {
    m_dFErr = dFErr;
    m_iMaxSteps = iMaxSteps;
    m_bFErr = true;
    m_pT = pT;
  }

  double
  find (const cfl::Function &rF, double dL, double dR) const
  {
    gsl_function uF;
    cfl::Function uG (rF);

    uF.function = &value;
    uF.params = &uG;

    std::unique_ptr<gsl_root_fsolver, decltype (&gsl_root_fsolver_free)>
        uSolver (gsl_root_fsolver_alloc (m_pT), &gsl_root_fsolver_free);

    gsl_root_fsolver_set (uSolver.get (), &uF, dL, dR);
    int iStatus = GSL_CONTINUE;
    double dX = 0.5 * (dL + dR);
    unsigned iSteps = 0;

    if (m_bFErr)
      {
        double dY;
        while ((iStatus == GSL_CONTINUE) && (iSteps < m_iMaxSteps))
          {
            iSteps++;
            gsl_root_fsolver_iterate (uSolver.get ());
            dX = gsl_root_fsolver_root (uSolver.get ());
            dY = GSL_FN_EVAL (&uF, dX);
            iStatus = gsl_root_test_residual (dY, m_dFErr);
          }
      }
    else
      {
        ASSERT (m_bFErr == false);

        while ((iStatus == GSL_CONTINUE) && (iSteps < m_iMaxSteps))
          {
            iSteps++;
            gsl_root_fsolver_iterate (uSolver.get ());
            dL = gsl_root_fsolver_x_lower (uSolver.get ());
            dR = gsl_root_fsolver_x_upper (uSolver.get ());
            iStatus = gsl_root_test_interval (dL, dR, m_dAbsErr, m_dRelErr);
          }
        dX = gsl_root_fsolver_root (uSolver.get ());
      }

    ASSERT ((iStatus == GSL_SUCCESS) || (iSteps == m_iMaxSteps));

    return dX;
  }

private:
  double m_dAbsErr, m_dRelErr, m_dFErr;
  unsigned m_iMaxSteps;
  bool m_bFErr;
  const gsl_root_fsolver_type *m_pT;
};
}

using namespace cflRoot;

// functions from NRoot

Root
cfl::NRoot::bisection (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return Root (new GSL_FSolver (dAbsErr, dRelErr, iMaxSteps,
                                gsl_root_fsolver_bisection));
}

Root
cfl::NRoot::bisection (double dFErr, unsigned iMaxSteps)
{
  return Root (new GSL_FSolver (dFErr, iMaxSteps, gsl_root_fsolver_bisection));
}

Root
cfl::NRoot::falsepos (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return Root (new GSL_FSolver (dAbsErr, dRelErr, iMaxSteps,
                                gsl_root_fsolver_falsepos));
}

Root
cfl::NRoot::falsepos (double dFErr, unsigned iMaxSteps)
{
  return Root (new GSL_FSolver (dFErr, iMaxSteps, gsl_root_fsolver_falsepos));
}

Root
cfl::NRoot::brent (double dAbsErr, double dRelErr, unsigned iMaxSteps)
{
  return Root (
      new GSL_FSolver (dAbsErr, dRelErr, iMaxSteps, gsl_root_fsolver_brent));
}

Root
cfl::NRoot::brent (double dFErr, unsigned iMaxSteps)
{
  return Root (new GSL_FSolver (dFErr, iMaxSteps, gsl_root_fsolver_brent));
}
