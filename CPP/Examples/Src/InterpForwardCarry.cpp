#include "Examples/Examples.hpp"
#include "cfl/Interp.hpp"

using namespace cfl;
using namespace std;

namespace NForwardCarryInterp
{
std::function<double (double, double)>
costOfCarry (double dSpot, double dInitialTime)
{
  return [dSpot, dInitialTime] (double dF, double dT) {
    PRECONDITION (dT > dInitialTime + cfl::EPS);

    return std::log (dF / dSpot) / (dT - dInitialTime);
  };
}
}; // namespace NForwardCarryInterp

using namespace NForwardCarryInterp;

cfl::Function
prb::forwardCarryInterp (double dSpot, const std::vector<double> &rTimes,
                         const std::vector<double> &rForwards,
                         double dInitialCarryRate, double dInitialTime,
                         cfl::Interp &rInterp)
{
  PRECONDITION (rTimes.size () == rForwards.size ());
  PRECONDITION (rTimes.size () > 0);
  PRECONDITION (rTimes.front () > dInitialTime);
  PRECONDITION (std::is_sorted (rTimes.begin (), rTimes.end (),
                                std::less_equal<double> ()));

  // times for interpolation = initial time + delivery times
  std::vector<double> uTimes (rTimes.size () + 1);
  uTimes.front () = dInitialTime;
  std::copy (rTimes.begin (), rTimes.end (), uTimes.begin () + 1);

  // market cost-of-carry rates
  std::vector<double> uCarry (uTimes.size ());
  uCarry.front () = dInitialCarryRate;
  std::transform (rForwards.begin (), rForwards.end (), rTimes.begin (),
                  uCarry.begin () + 1, costOfCarry (dSpot, dInitialTime));

  // interpolation of the cost-of-carry rates
  rInterp.assign (uTimes.begin (), uTimes.end (), uCarry.begin ());
  Function uCarryFunction = rInterp.interp ();

  // return forward curve
  Function uT ([dInitialTime] (double dT) { return dT - dInitialTime; },
               dInitialTime);

  return dSpot * exp (uCarryFunction * uT);
}
