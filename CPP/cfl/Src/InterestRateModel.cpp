#include "cfl/InterestRateModel.hpp"

using namespace cfl;

cfl::InterestRateModel::InterestRateModel (IInterestRateModel *pNewModel)
    : m_pModel (pNewModel)
{
}
