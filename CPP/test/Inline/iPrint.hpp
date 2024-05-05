// do not include this file
template <class T>
void
test::print (T start, T end, const std::string &rName)
{
  std::string sM (rName);
  sM += std::string (":");
  std::cout << sM.c_str () << std::endl;
  std::function<double (double)> uRound = roundResult ();

  for (T i = start; i < end; i++)
    {
      std::cout << "[" << (i - start) << "]"
                << " = " << uRound (*i) << std::endl;
    }
  std::cout << std::endl;
}
