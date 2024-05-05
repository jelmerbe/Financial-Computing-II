#ifndef __test_all_Slice_hpp__
#define __test_all_Slice_hpp__

#include "cfl/Slice.hpp"
#include "test/Main.hpp"
#include <functional>

namespace test
{
/**
 * Prints the values of Slice.
 *
 * @param rSlice Input.
 * @param rName The printed name of the slice.
 * @param iMaxRows The maximal number of rows.
 */
void print (const cfl::Slice &rSlice, const std::string &rName,
            unsigned iMaxRows = 9);

/**
 * Compares numerical and exact results.
 *
 * @param rExact The exact slice.
 * @param rApprox The approximate slice.
 * @param rTitle The title of the table.
 * @param iColumn The size of every column.
 * @param iSpace The space between columns.
 * @param iMaxRows The maximal number of rows.
 */
void compare (const cfl::Slice &rExact, const cfl::Slice &rApprox,
              const std::string &rTitle, unsigned iColumn = 10,
              unsigned iSpace = 10, unsigned iMaxRows = 9);
} // namespace test

#endif // of _test_all_Slice_hpp__
