#include "cfl/Similar.hpp"
#include "cfl/Slice.hpp"

using namespace cfl;
using namespace std;

class TargetModel : public IModel
{
public:
  TargetModel (const TRollback &rRollback, const Model &rModel)
      : m_uRollback (rRollback), m_uModel (rModel)
  {
  }

  const std::vector<double> &
  eventTimes () const
  {
    return model ().eventTimes ();
  }

  unsigned
  numberOfStates () const
  {
    return model ().numberOfStates ();
  }

  Slice
  state (unsigned iTime, unsigned iState) const
  {
    Slice uState = model ().state (iTime, iState);
    uState.assign (*this);

    return uState;
  }

  unsigned
  numberOfNodes (unsigned iTime,
                 const std::vector<unsigned> &rDependence) const
  {
    return model ().numberOfNodes (iTime, rDependence);
  }

  std::valarray<double>
  origin () const
  {
    return model ().origin ();
  }

  void
  addDependence (Slice &rSlice, const std::vector<unsigned> &rDependence) const
  {
    rSlice.assign (model ());
    model ().addDependence (rSlice, rDependence);
    rSlice.assign (*this);
  }

  void
  rollback (Slice &rSlice, unsigned iTime) const
  {
    rSlice.assign (model ());
    m_uRollback (rSlice, iTime);
    rSlice.assign (*this);
  }

  void
  indicator (Slice &rSlice, double dBarrier) const
  {
    rSlice.assign (model ());
    m_uModel.model ().indicator (rSlice, dBarrier);
    rSlice.assign (*this);
  }

  MultiFunction
  interpolate (const Slice &rSlice) const
  {
    Slice uSlice (rSlice);
    uSlice.assign (model ());

    return model ().interpolate (uSlice);
  }

private:
  const IModel &
  model () const
  {
    return m_uModel.model ();
  }

  TRollback m_uRollback;
  Model m_uModel;
};

Model
cfl::similar (const TRollback &rTargetRollback, const Model &rBase)
{
  return Model (new TargetModel (rTargetRollback, rBase));
}
