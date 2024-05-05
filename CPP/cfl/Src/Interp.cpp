#include "cfl/Interp.hpp"
#include "cfl/Error.hpp"
#include <gsl/gsl_spline.h>

using namespace cfl;

// class Interp

cfl::Interp::Interp (IInterp *pNewP) : m_uP (pNewP) {}

// class Interp_GSL

class Interp_GSL : public IInterp
{
public:
  Interp_GSL (const gsl_interp_type *pT) : m_pT (pT) {}

  Interp_GSL (const std::vector<double> &rArg, const std::vector<double> &rVal,
              const gsl_interp_type *pT)
      : m_pT (pT), m_dL (rArg.front ()), m_dR (rArg.back ())
  {
    PRECONDITION ((rArg.size () == rVal.size ()) && (rArg.size () >= 2));
    PRECONDITION (
        is_sorted (rArg.begin (), rArg.end (), std::less_equal<double> ()));

    // if size is not sufficient we do linear interpolation
    const gsl_interp_type *pInterpT
        = (rArg.size () > gsl_interp_type_min_size (m_pT)) ? m_pT
                                                           : gsl_interp_linear;
    m_uS.reset (gsl_spline_alloc (pInterpT, rArg.size ()), &gsl_spline_free);
    // copies of rArg and rVal will be created
    gsl_spline_init (m_uS.get (), rArg.data (), rVal.data (), rArg.size ());
  }

  IInterp *
  newObject (const std::vector<double> &rArg,
             const std::vector<double> &rVal) const
  {
    return new Interp_GSL (rArg, rVal, m_pT);
  }

  Function
  interp () const
  {
    std::shared_ptr<gsl_interp_accel> uAcc (gsl_interp_accel_alloc (),
                                            &gsl_interp_accel_free);

    std::function<double (double)> uF = [uS = m_uS, uAcc] (double dX) {
      return gsl_spline_eval (uS.get (), dX, uAcc.get ());
    };

    return Function (uF, m_dL, m_dR);
  }

  Function
  deriv () const
  {
    std::shared_ptr<gsl_interp_accel> uAcc (gsl_interp_accel_alloc (),
                                            &gsl_interp_accel_free);

    std::function<double (double)> uF = [uS = m_uS, uAcc] (double dX) {
      return gsl_spline_eval_deriv (uS.get (), dX, uAcc.get ());
    };

    return Function (uF, m_dL, m_dR);
  }

  Function
  deriv2 () const
  {
    std::shared_ptr<gsl_interp_accel> uAcc (gsl_interp_accel_alloc (),
                                            &gsl_interp_accel_free);

    std::function<double (double)> uF = [uS = m_uS, uAcc] (double dX) {
      return gsl_spline_eval_deriv2 (uS.get (), dX, uAcc.get ());
    };

    return Function (uF, m_dL, m_dR);
  }

private:
  const gsl_interp_type *m_pT;
  std::shared_ptr<gsl_spline> m_uS;
  double m_dL, m_dR;
};

// functions from NInterp

cfl::Interp
cfl::NInterp::linear ()
{
  return Interp (new Interp_GSL (gsl_interp_linear));
}

cfl::Interp
cfl::NInterp::cspline ()
{
  return Interp (new Interp_GSL (gsl_interp_cspline));
}

cfl::Interp
cfl::NInterp::steffen ()
{
  return Interp (new Interp_GSL (gsl_interp_steffen));
}

cfl::Interp
cfl::NInterp::akima ()
{
  return Interp (new Interp_GSL (gsl_interp_akima));
}

cfl::Interp
cfl::NInterp::polynomial ()
{
  return Interp (new Interp_GSL (gsl_interp_polynomial));
}
