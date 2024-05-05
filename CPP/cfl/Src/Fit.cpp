#include "cfl/Fit.hpp"
#include "cfl/Error.hpp"
#include <algorithm>
#include <cmath>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_bspline.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit.h>
#include <numeric>

using namespace cfl;
using namespace std;

// class Fit

cfl::Fit::Fit (IFit *pNewP) : m_uP (pNewP) {}

// Checking domains

bool
belongs (const std::vector<Function> &rBaseF, const Function &rFreeF,
         double dX)
{
  double bA
      = std::all_of (rBaseF.begin (), rBaseF.end (),
                     [dX] (const Function &rG) { return rG.belongs (dX); });
  bA = bA && rFreeF.belongs (dX);

  return bA;
}

bool
belongs (const std::vector<Function> &rBaseF, const Function &rFreeF,
         const std::vector<double> &rArg)
{
  return std::all_of (
      rArg.begin (), rArg.end (),
      [&rBaseF, &rFreeF] (double dX) { return belongs (rBaseF, rFreeF, dX); });
}

FitParam
fitParam (const gsl_vector *pC, const gsl_matrix *pCov, double dChi2)
{
  FitParam uParam;
  unsigned iSize = pC->size;
  uParam.fit = std::valarray<double> (pC->data, iSize);
  uParam.cov = std::valarray<double> (pCov->data, iSize * iSize);
  uParam.chi2 = dChi2;

  return uParam;
}

// class LinFit

class LinFit : public cfl::IFit
{
public:
  LinFit (const std::vector<Function> &rBaseF, const Function &rFreeF)
      : m_uBaseF (rBaseF), m_uFreeF (rFreeF),
        m_uCov (gsl_matrix_alloc (rBaseF.size (), rBaseF.size ()),
                &gsl_matrix_free),
        m_uC (gsl_vector_alloc (rBaseF.size ()), &gsl_vector_free)
  {
    PRECONDITION (rBaseF.size () > 0);
  }

  LinFit (const std::vector<Function> &rBaseF, const Function &rFreeF,
          const std::vector<double> &rArg, const std::vector<double> &rVal,
          const std::vector<double> &rWt, bool bChi2)
      : LinFit (rBaseF, rFreeF)
  {
    PRECONDITION (belongs (rBaseF, rFreeF, rArg));
    PRECONDITION (std::all_of (rWt.begin (), rWt.end (),
                               [] (double dW) { return dW > 0; }));
    PRECONDITION ((rArg.size () == rVal.size ()) && (rArg.size () > 0)
                  && (rArg.size () == rWt.size ()));
    PRECONDITION (rBaseF.size () < rArg.size ());

    if (rArg.size () <= rBaseF.size ())
      {
        throw (cfl::NError::size ("not enough nodes for linear fit"));
      }

    std::unique_ptr<gsl_multifit_linear_workspace,
                    decltype (&gsl_multifit_linear_free)>
        uFit (gsl_multifit_linear_alloc (rArg.size (), m_uBaseF.size ()),
              &gsl_multifit_linear_free);

    std::vector<double> uX (rArg.size () * rBaseF.size ());
    std::vector<double>::iterator itRowX = uX.begin ();
    std::for_each (rArg.begin (), rArg.end (), [&itRowX, &rBaseF] (double dT) {
      std::transform (rBaseF.begin (), rBaseF.end (), itRowX,
                      [dT] (const cfl::Function &rF) { return rF (dT); });
      itRowX += rBaseF.size ();
    });

    ASSERT (itRowX == uX.end ());

    gsl_matrix_const_view uXv = gsl_matrix_const_view_array (
        uX.data (), rArg.size (), rBaseF.size ());

    gsl_vector_const_view uW
        = gsl_vector_const_view_array (rWt.data (), rWt.size ());

    std::vector<double> uY (rArg.size ());
    std::transform (
        rArg.begin (), rArg.end (), rVal.begin (), uY.begin (),
        [&rFreeF] (double dX, double dY) { return dY - rFreeF (dX); });
    gsl_vector_const_view uYv
        = gsl_vector_const_view_array (uY.data (), uY.size ());

    gsl_multifit_wlinear (&uXv.matrix, &uW.vector, &uYv.vector, m_uC.get (),
                          m_uCov.get (), &m_dChi2, uFit.get ());

    if (bChi2)
      {
        double dVar = m_dChi2 / (rArg.size () - rBaseF.size ());
        gsl_matrix_scale (m_uCov.get (), dVar);
      }
  }

  IFit *
  newObject (const std::vector<double> &rArg, const std::vector<double> &rVal,
             const std::vector<double> &rWt, bool bChi2) const
  {
    return new LinFit (m_uBaseF, m_uFreeF, rArg, rVal, rWt, bChi2);
  }

  Function
  fit () const
  {
    std::function<double (double)> uFit
        = [uBase = m_uBaseF, uG = m_uFreeF, uC = m_uC] (double dX) {
            std::vector<double> uV (uBase.size ());

            std::transform (uBase.begin (), uBase.end (), uV.begin (),
                            [dX] (const Function &rF) { return rF (dX); });
            gsl_vector_const_view uU
                = gsl_vector_const_view_array (uV.data (), uV.size ());
            double dY;
            gsl_blas_ddot (&uU.vector, uC.get (), &dY);
            dY += uG (dX);

            return dY;
          };

    return Function (uFit, [uBase = m_uBaseF, uG = m_uFreeF] (double dX) {
      return belongs (uBase, uG, dX);
    });
  }

  Function
  err () const
  {
    std::function<double (double)> uErr = [uBase = m_uBaseF,
                                           uCov = m_uCov] (double dX) {
      std::vector<double> uV (uBase.size ());

      std::transform (uBase.begin (), uBase.end (), uV.begin (),
                      [dX] (const Function &rF) { return rF (dX); });

      std::vector<double> uU (uV);
      gsl_vector_const_view uUView
          = gsl_vector_const_view_array (uU.data (), uU.size ());
      gsl_vector_view uVView = gsl_vector_view_array (uV.data (), uV.size ());
      gsl_blas_dsymv (CblasUpper, 1., uCov.get (), &uUView.vector, 0.,
                      &uVView.vector);
      gsl_blas_ddot (&uUView.vector, &uVView.vector, &dX);

      ASSERT (dX >= 0);

      return std::sqrt (dX);
    };

    return Function (uErr, [uBase = m_uBaseF, uG = m_uFreeF] (double dX) {
      return belongs (uBase, uG, dX);
    });
  }

  FitParam
  param () const
  {
    return fitParam (m_uC.get (), m_uCov.get (), m_dChi2);
  }

private:
  std::vector<Function> m_uBaseF;
  Function m_uFreeF;
  std::shared_ptr<gsl_matrix> m_uCov;
  std::shared_ptr<gsl_vector> m_uC;
  double m_dChi2;
};

// linear multi-dim

cfl::Fit
cfl::NFit::linear (const std::vector<Function> &rBaseF, const Function &rFreeF)
{
  return Fit (new LinFit (rBaseF, rFreeF));
}

// class OneDimFit

class OneDimFit : public IFit
{
public:
  OneDimFit (const Function &rBaseF, const Function rFreeF)
      : m_uBaseF (rBaseF), m_uFreeF (rFreeF)
  {
  }

  OneDimFit (const Function &rBaseF, const Function rFreeF,
             const std::vector<double> &rArg, const std::vector<double> &rVal,
             const std::vector<double> &rWt, bool bChi2)
      : OneDimFit (rBaseF, rFreeF)
  {
    PRECONDITION (
        std::all_of (rArg.begin (), rArg.end (),
                     [&rBaseF] (double dX) { return rBaseF.belongs (dX); }));
    PRECONDITION (std::all_of (rWt.begin (), rWt.end (),
                               [] (double dW) { return dW > 0; }));
    PRECONDITION ((rArg.size () == rVal.size ())
                  && (rArg.size () == rWt.size ()));
    PRECONDITION (rArg.size () > 1);

    std::vector<double> uX (rArg.size ());
    std::transform (rArg.begin (), rArg.end (), uX.begin (),
                    [&rBaseF] (double dT) { return rBaseF (dT); });

    std::vector<double> uY (rArg.size ());
    std::transform (
        rArg.begin (), rArg.end (), rVal.begin (), uY.begin (),
        [&rFreeF] (double dX, double dY) { return dY - rFreeF (dX); });

    gsl_fit_wmul (uX.data (), 1, rWt.data (), 1, uY.data (), 1, rArg.size (),
                  &m_dC, &m_dVar, &m_dChi2);

    if (bChi2)
      {
        double dVar = m_dChi2 / (rArg.size () - 1);
        m_dVar *= dVar;
      }
  }

  IFit *
  newObject (const std::vector<double> &rArg, const std::vector<double> &rVal,
             const std::vector<double> &rWt, bool bChi2) const
  {
    return new OneDimFit (m_uBaseF, m_uFreeF, rArg, rVal, rWt, bChi2);
  }

  Function
  fit () const
  {
    return m_dC * m_uBaseF + m_uFreeF;
  }

  Function
  err () const
  {
    return sqrt (m_dVar * m_uBaseF * m_uBaseF);
  }

  FitParam
  param () const
  {
    FitParam uParam;
    uParam.fit = std::valarray<double> (m_dC, 1);
    uParam.cov = std::valarray<double> (m_dVar, 1);
    uParam.chi2 = m_dChi2;

    return uParam;
  }

private:
  Function m_uBaseF, m_uFreeF;
  double m_dC, m_dVar, m_dChi2;
};

// linear one-dim

cfl::Fit
cfl::NFit::linear (const cfl::Function &rBaseF, const cfl::Function &rFreeF)
{
  return Fit (new OneDimFit (rBaseF, rFreeF));
}

// class Regression

class Regression : public IFit
{
public:
  Regression (const Function &rBaseF, const Function rFreeF)
      : m_uBaseF (rBaseF), m_uFreeF (rFreeF)
  {
  }

  Regression (const Function &rBaseF, const Function rFreeF,
              const std::vector<double> &rArg, const std::vector<double> &rVal,
              const std::vector<double> &rWt, bool bChi2)
      : Regression (rBaseF, rFreeF)
  {
    PRECONDITION (
        std::all_of (rArg.begin (), rArg.end (),
                     [&rBaseF] (double dX) { return rBaseF.belongs (dX); }));
    PRECONDITION (std::all_of (rWt.begin (), rWt.end (),
                               [] (double dW) { return dW > 0; }));
    PRECONDITION ((rArg.size () == rVal.size ())
                  && (rArg.size () == rWt.size ()));
    PRECONDITION (rArg.size () > 1);

    std::vector<double> uX (rArg.size ());
    std::transform (rArg.begin (), rArg.end (), uX.begin (),
                    [&rBaseF] (double dT) { return rBaseF (dT); });

    std::vector<double> uY (rArg.size ());
    std::transform (
        rArg.begin (), rArg.end (), rVal.begin (), uY.begin (),
        [&rFreeF] (double dX, double dY) { return dY - rFreeF (dX); });

    m_uParam.fit.resize (2);
    m_uParam.cov.resize (4);

    double *pC0 = &m_uParam.fit[0];
    double *pC1 = &m_uParam.fit[1];
    double *pCov00 = &m_uParam.cov[0];
    double *pCov01 = &m_uParam.cov[1];
    double *pCov11 = &m_uParam.cov[3];
    double *pChi2 = &m_uParam.chi2;

    gsl_fit_wlinear (uX.data (), 1, rWt.data (), 1, uY.data (), 1,
                     rArg.size (), pC0, pC1, pCov00, pCov01, pCov11, pChi2);

    m_uParam.cov[2] = m_uParam.cov[1];

    if (bChi2)
      {
        double dVar = m_uParam.chi2 / (rArg.size () - 2.);
        m_uParam.cov *= dVar;
      }
  }

  IFit *
  newObject (const std::vector<double> &rArg, const std::vector<double> &rVal,
             const std::vector<double> &rWt, bool bChi2) const
  {
    return new Regression (m_uBaseF, m_uFreeF, rArg, rVal, rWt, bChi2);
  }

  Function
  fit () const
  {
    return m_uParam.fit[0] + m_uParam.fit[1] * m_uBaseF + m_uFreeF;
  }

  Function
  err () const
  {
    std::function<double (double)> uErr = [uBase = m_uBaseF,
                                           uCov = m_uParam.cov] (double dX) {
      std::vector<double> uV (2);
      uV.front () = 1.;
      uV.back () = uBase (dX);

      std::vector<double> uU (uV);
      gsl_vector_const_view uUView
          = gsl_vector_const_view_array (uU.data (), uU.size ());
      gsl_vector_view uVView = gsl_vector_view_array (uV.data (), uV.size ());
      gsl_matrix_const_view uCovView
          = gsl_matrix_const_view_array (&uCov[0], 2, 2);
      gsl_blas_dsymv (CblasUpper, 1., &uCovView.matrix, &uUView.vector, 0.,
                      &uVView.vector);
      gsl_blas_ddot (&uUView.vector, &uVView.vector, &dX);

      ASSERT (dX >= 0);

      return std::sqrt (dX);
    };

    return Function (uErr, [uBase = m_uBaseF, uG = m_uFreeF] (double dX) {
      return uBase.belongs (dX) && uG.belongs (dX);
    });
  }

  FitParam
  param () const
  {
    return m_uParam;
  }

private:
  Function m_uBaseF, m_uFreeF;
  FitParam m_uParam;
};

// linear regression

cfl::Fit
cfl::NFit::linear_regression (const cfl::Function &rBaseF,
                              const cfl::Function &rFreeF)
{
  return Fit (new Regression (rBaseF, rFreeF));
}

// class BSpline

class BSpline : public IFit
{
public:
  BSpline (unsigned iOrder, const std::vector<double> &rPoints)
      : m_iOrder (iOrder), m_uPoints (rPoints)
  {
    m_iF = iOrder + rPoints.size () - 2;
    m_uBSpline.reset (gsl_bspline_alloc (iOrder, rPoints.size ()),
                      &gsl_bspline_free);

    ASSERT (m_iF == gsl_bspline_ncoeffs (m_uBSpline.get ()));

    m_uCov.reset (gsl_matrix_alloc (m_iF, m_iF), &gsl_matrix_free);
    m_uC.reset (gsl_vector_alloc (m_iF), &gsl_vector_free);
    gsl_vector_const_view uVec
        = gsl_vector_const_view_array (rPoints.data (), rPoints.size ());
    gsl_bspline_knots (&uVec.vector, m_uBSpline.get ());
  }

  BSpline (unsigned iOrder, const std::vector<double> &rPoints,
           const std::vector<double> &rArg, const std::vector<double> &rVal,
           const std::vector<double> &rWt, bool bChi2)
      : BSpline (iOrder, rPoints)
  {
    PRECONDITION ((rArg.size () == rVal.size ()) && (rArg.size () > 0));
    PRECONDITION (rArg.size () == rWt.size ());
    PRECONDITION (rArg.size () > m_iF);

    if (rArg.size () <= m_iF)
      {
        throw (NError::size ("not enough nodes for fitting with B-splines"));
      }

    std::unique_ptr<gsl_multifit_linear_workspace,
                    decltype (&gsl_multifit_linear_free)>
        uFit (gsl_multifit_linear_alloc (rArg.size (), m_iF),
              &gsl_multifit_linear_free);

    std::unique_ptr<gsl_matrix, decltype (&gsl_matrix_free)> uX (
        gsl_matrix_alloc (rArg.size (), m_iF), &gsl_matrix_free);
    for (unsigned i = 0; i < rArg.size (); i++)
      {
        gsl_vector_view uV = gsl_matrix_row (uX.get (), i);
        gsl_bspline_eval (rArg[i], &uV.vector, m_uBSpline.get ());
      }
    gsl_vector_const_view uW
        = gsl_vector_const_view_array (rWt.data (), rWt.size ());
    gsl_vector_const_view uY
        = gsl_vector_const_view_array (rVal.data (), rVal.size ());

    gsl_multifit_wlinear (uX.get (), &uW.vector, &uY.vector, m_uC.get (),
                          m_uCov.get (), &m_dChi2, uFit.get ());

    if (bChi2)
      {
        double dVar = m_dChi2 / (rArg.size () - m_iF);
        gsl_matrix_scale (m_uCov.get (), dVar);
      }
  }

  IFit *
  newObject (const std::vector<double> &rArg, const std::vector<double> &rVal,
             const std::vector<double> &rWt, bool bChi2) const
  {
    return new BSpline (m_iOrder, m_uPoints, rArg, rVal, rWt, bChi2);
  }

  Function
  fit () const
  {
    std::function<double (double)> uFit = [uC = m_uC, uBS = m_uBSpline,
                                           iK = m_iOrder] (double dX) {
      std::vector<double> uF (iK);
      gsl_vector_view uViewF = gsl_vector_view_array (uF.data (), uF.size ());
      unsigned long iStart, iEnd;
      gsl_bspline_eval_nonzero (dX, &uViewF.vector, &iStart, &iEnd,
                                uBS.get ());

      ASSERT (iStart + iK == iEnd + 1);

      gsl_vector_view uViewG = gsl_vector_subvector (uC.get (), iStart, iK);
      gsl_blas_ddot (&uViewG.vector, &uViewF.vector, &dX);
      return dX;
    };
    return Function (uFit, m_uPoints.front (), m_uPoints.back ());
  }

  Function
  err () const
  {
    std::function<double (double)> uErr = [uCov = m_uCov, uB = m_uBSpline,
                                           iK = m_iOrder] (double dX) {
      PRECONDITION (uCov->size1 == uCov->size2);

      std::vector<double> uF (iK);
      gsl_vector_view uFView = gsl_vector_view_array (uF.data (), uF.size ());
      unsigned long iStart, iEnd;
      gsl_bspline_eval_nonzero (dX, &uFView.vector, &iStart, &iEnd, uB.get ());

      ASSERT (iStart + iK == iEnd + 1);

      std::vector<double> uG (uF);
      gsl_vector_view uGView = gsl_vector_view_array (uG.data (), uG.size ());
      gsl_matrix_view uCovView
          = gsl_matrix_submatrix (uCov.get (), iStart, iStart, iK, iK);
      gsl_blas_dsymv (CblasUpper, 1., &uCovView.matrix, &uGView.vector, 0.,
                      &uFView.vector);
      gsl_blas_ddot (&uFView.vector, &uGView.vector, &dX);

      return std::sqrt (dX);
    };

    return Function (uErr, m_uPoints.front (), m_uPoints.back ());
  }

  FitParam
  param () const
  {
    return fitParam (m_uC.get (), m_uCov.get (), m_dChi2);
  }

private:
  unsigned m_iOrder, m_iF;
  std::vector<double> m_uPoints;
  std::shared_ptr<gsl_bspline_workspace> m_uBSpline;
  std::shared_ptr<gsl_matrix> m_uCov;
  std::shared_ptr<gsl_vector> m_uC;
  double m_dChi2;
};

// bspline

cfl::Fit
cfl::NFit::bspline (unsigned iOrder, const std::vector<double> &rPoints)
{
  return Fit (new BSpline (iOrder, rPoints));
}

cfl::Fit
cfl::NFit::bspline (unsigned iOrder, double dL, double dR, unsigned iPoints)
{
  PRECONDITION (dL < dR);
  PRECONDITION (iPoints > 1);

  double dS = (dR - dL) / (iPoints - 1.);
  std::vector<double> uPoints (iPoints, dL);
  uPoints.back () = dR;
  std::transform (uPoints.begin (), uPoints.end () - 2, uPoints.begin () + 1,
                  [dS] (double dX) { return dX + dS; });

  POSTCONDITION (std::is_sorted (uPoints.begin (), uPoints.end (),
                                 std::less_equal<double> ()));
  POSTCONDITION (uPoints.front () == dL);
  POSTCONDITION (uPoints.back () == dR);

  return Fit (new BSpline (iOrder, uPoints));
}
