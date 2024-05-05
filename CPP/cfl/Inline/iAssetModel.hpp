// do not include this file

inline void
cfl::AssetModel::assignEventTimes (const std::vector<double> &rEventTimes)
{
  // initial time should be the same
  PRECONDITION (rEventTimes.front () == eventTimes ().front ());

  m_pModel.reset (m_pModel->newModel (rEventTimes));
}

inline const cfl::IModel &
cfl::AssetModel::model () const
{
  return m_pModel->model ();
}

inline const std::vector<double> &
cfl::AssetModel::eventTimes () const
{
  return model ().eventTimes ();
}

inline double
cfl::AssetModel::initialTime () const
{
  return eventTimes ().front ();
}

inline cfl::Slice
cfl::AssetModel::cash (unsigned iTime, double dAmount) const
{
  PRECONDITION (iTime < eventTimes ().size ());

  return Slice (&model (), iTime, dAmount);
}

inline cfl::Slice
cfl::AssetModel::discount (unsigned iTime, double dBondMaturity) const
{
  PRECONDITION (iTime < eventTimes ().size ());
  PRECONDITION (eventTimes ()[iTime] <= dBondMaturity);

  return m_pModel->discount (iTime, dBondMaturity);
}

inline cfl::Slice
cfl::AssetModel::forward (unsigned iTime, double dForwardMaturity) const
{
  PRECONDITION (iTime < eventTimes ().size ());
  PRECONDITION (eventTimes ()[iTime] <= dForwardMaturity);

  return m_pModel->forward (iTime, dForwardMaturity);
}

inline cfl::Slice
cfl::AssetModel::spot (unsigned iTime) const
{
  PRECONDITION (iTime < eventTimes ().size ());

  return forward (iTime, eventTimes ()[iTime]);
}

inline cfl::Slice
cfl::AssetModel::state (unsigned iTime, unsigned iState) const
{
  PRECONDITION (iTime < eventTimes ().size ());
  PRECONDITION (iState < model ().numberOfStates ());

  return model ().state (iTime, iState);
}
