#include "cfl/Model.hpp"
#include "cfl/Slice.hpp"

using namespace cfl;

// class Model

cfl::Model::Model (IModel *pNewModel) : m_pModel (pNewModel) {}

cfl::Slice
cfl::Model::state (unsigned iEventTime, unsigned iState) const
{
  return m_pModel->state (iEventTime, iState);
}
