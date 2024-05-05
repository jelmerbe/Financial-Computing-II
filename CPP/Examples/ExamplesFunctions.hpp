#ifndef __Examples_Functions_hpp__
#define __Examples_Functions_hpp__

namespace NExamples
{
double shape1 (double dX);
double shape2 (double dX);
cfl::Slice swap (unsigned iTime, const cfl::Data::Swap &rSwap,
                 const cfl::InterestRateModel &rModel);
}

#endif // of __Examples_Functions_hpp__
