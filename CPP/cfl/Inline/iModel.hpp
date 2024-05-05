// do not include this file

inline const cfl::IModel &
cfl::Model::model () const
{
  return *m_pModel;
}

inline const std::vector<double> &
cfl::Model::eventTimes () const
{
  return m_pModel->eventTimes ();
}