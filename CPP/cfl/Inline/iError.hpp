// do not include this file

inline const char *
cfl::Error::what () const throw ()
{
  return m_sErr.data ();
}
