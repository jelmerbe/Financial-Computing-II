#include "cfl/Error.hpp"

using namespace cfl;

// CLASS: Error

cfl::Error::Error (const char *pWhere) : m_sErr (pWhere) {}

namespace cflError
{
// FUNCTION: basic

cfl::Error
basic (const std::string &sType, const char *pWhere)
{
  std::string sErr (sType);
  sErr += (": ");
  sErr += pWhere;
  return Error (sErr.data ());
}
}

using namespace cflError;

// FUNCTION: range

const std::string c_sRange ("out of range");

Error
NError::range (const char *pWhere)
{
  return basic (c_sRange, pWhere);
}

// FUNCTION: size

const std::string c_sSize ("invalid size");

Error
NError::size (const char *pWhere)
{
  return basic (c_sSize, pWhere);
}

// FUNCTION: sort

const std::string c_sSort ("invalid order");

Error
NError::sort (const char *pWhere)
{
  return basic (c_sSort, pWhere);
}
