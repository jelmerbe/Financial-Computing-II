#include "Examples/Examples.hpp"
#include "Examples/ExamplesFunctions.hpp"
#include <numeric>

using namespace cfl;
using namespace std;
using namespace prb;
using namespace cfl::Data;

double
prb::callsImplVol (const std::vector<Option> &rCalls, double dPrice,
                   const cfl::Function &rDiscount,
                   const cfl::Function &rForward, double dInitialTime,
                   double dSigma0, double dSigma1, const cfl::Root &rRoot)
{
  PRECONDITION (std::all_of (rCalls.begin (), rCalls.end (),
                             [dInitialTime] (const Option &rCall) {
                               return rCall.maturity > dInitialTime;
                             }));
  PRECONDITION (dSigma0 < dSigma1);

  std::function<double (double)> uDiff =
      [&rCalls, dPrice, &rDiscount, &rForward, dInitialTime] (double dSigma) {
        return std::accumulate (
            rCalls.begin (), rCalls.end (), -dPrice,
            [dInitialTime, &rDiscount, &rForward,
             dSigma] (double dSum, const Option &rCall) {
              double dT = rCall.maturity;
              dSum += rCall.number
                      * callBlack (rCall.strike, dT - dInitialTime,
                                   rDiscount (dT), rForward (dT), dSigma);

              return dSum;
            });
      };

  Function uF (uDiff, dSigma0, dSigma1);

  return rRoot.find (uF, dSigma0, dSigma1);
}
