// do not include this file

inline void
cfl::InterestRateModel::assignEventTimes (
    const std::vector<double> &rEventTimes)
{
  // initial time should be the same
  PRECONDITION (rEventTimes.front () == initialTime ());

  m_pModel.reset (m_pModel->newModel (rEventTimes));
}

inline const cfl::IModel &
cfl::InterestRateModel::model () const
{
  return m_pModel->model ();
}

inline const std::vector<double> &
cfl::InterestRateModel::eventTimes () const
{
  return model ().eventTimes ();
}

inline double
cfl::InterestRateModel::initialTime () const
{
  return eventTimes ().front ();
}

inline cfl::Slice
cfl::InterestRateModel::cash (unsigned iTime, double dAmount) const
{
  PRECONDITION (iTime < eventTimes ().size ());

  return Slice (&model (), iTime, dAmount);
}

inline cfl::Slice
cfl::InterestRateModel::discount (unsigned iTime, double dBondMaturity) const
{
  PRECONDITION (iTime < eventTimes ().size ());
  PRECONDITION (eventTimes ()[iTime] <= dBondMaturity);

  return m_pModel->discount (iTime, dBondMaturity);
}

inline cfl::Slice
cfl::InterestRateModel::state (unsigned iTime, unsigned iState) const
{
  PRECONDITION (iTime < eventTimes ().size ());
  PRECONDITION (iState < model ().numberOfStates ());

  return model ().state (iTime, iState);
}
