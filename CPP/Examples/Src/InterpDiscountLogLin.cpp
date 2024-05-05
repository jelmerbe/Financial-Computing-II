#include "Examples/Examples.hpp"
#include "cfl/Interp.hpp"
#include "cfl/Macros.hpp"
#include <cmath>

using namespace cfl;
using namespace std;

cfl::Function
prb::discountLogLinInterp (const std::vector<double> &rTimes,
                           const std::vector<double> &rDiscount,
                           double dInitialTime)
{
  PRECONDITION (rTimes.size () == rDiscount.size ());
  PRECONDITION (rTimes.size () > 0);
  PRECONDITION (rTimes.front () > dInitialTime);
  PRECONDITION (std::is_sorted (rTimes.begin (), rTimes.end (),
                                std::less_equal<double> ()));

  // times for interpolation = initial time + discount times
  std::vector<double> uTimes (rTimes.size () + 1);
  uTimes.front () = dInitialTime;
  std::copy (rTimes.begin (), rTimes.end (), uTimes.begin () + 1);

  // logs of discount factors
  std::vector<double> uLogDiscount (uTimes.size ());
  uLogDiscount.front () = 0.;
  std::transform (rDiscount.begin (), rDiscount.end (),
                  uLogDiscount.begin () + 1,
                  [] (double dX) { return std::log (dX); });

  // linear interpolation of the logs of discount factors
  cfl::Interp uInterp = NInterp::linear ();
  uInterp.assign (uTimes.begin (), uTimes.end (), uLogDiscount.begin ());
  Function uLogDiscountFunction = uInterp.interp ();

  return cfl::exp (uLogDiscountFunction);
}
