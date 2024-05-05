#include "Examples/Examples.hpp"

using namespace cfl;
using namespace std;

cfl::Function
prb::forwardFX (double dSpotFX, const cfl::Function &rDomesticDiscount,
                const cfl::Function &rForeignDiscount)
{
  return dSpotFX * rForeignDiscount / rDomesticDiscount;
}
