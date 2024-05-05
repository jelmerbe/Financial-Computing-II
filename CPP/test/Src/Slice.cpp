#include "cfl/Slice.hpp"
#include "cfl/Brownian.hpp"
#include "test/Slice.hpp"
#include <algorithm>
#include <functional>

using namespace cfl;
using namespace std;
using namespace test;

void
test::print (const cfl::Slice &rSlice, const std::string &rName,
             unsigned iMaxRows)
{
  std::valarray<double> uValues (rSlice.values ());
  unsigned iSize = std::min<unsigned> (uValues.size (), iMaxRows);
  unsigned iStart = (iSize < iMaxRows) ? 0 : (uValues.size () - iMaxRows) / 2;
  test::print (begin (uValues) + iStart, begin (uValues) + iStart + iSize,
               rName);
  if (rSlice.timeIndex () == 0)
    {
      print (atOrigin (rSlice)[0], "value at origin", true);
    }
}

void
test::compare (const cfl::Slice &rExact, const cfl::Slice &rApprox,
               const std::string &rTitle, unsigned iColumn, unsigned iSpace,
               unsigned iMaxRows)
{
  PRECONDITION (rExact.timeIndex () == rApprox.timeIndex ());

  compare (rExact.values (), rApprox.values (), rTitle, iColumn, iSpace,
           iMaxRows);
  unsigned iTime = rExact.timeIndex ();
  if (iTime == 0)
    {
      double dErr = std::abs (atOrigin (rApprox) - atOrigin (rExact))[0];
      print (dErr, "error at origin", true);
    }
}
