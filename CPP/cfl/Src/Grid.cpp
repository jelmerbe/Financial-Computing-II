#include "cfl/Grid.hpp"

std::function<double (double)>
cfl::Grid::widthGauss (double dWidthQuality)
{
  return [dWidthQuality] (double dVar) {
    double dW
        = 2.
              * (dVar
                 + std::sqrt (dVar * (dVar + 4. * std::log (dWidthQuality))))
          + cfl::EPS;

    POSTCONDITION (dW > 0);

    return dW;
  };
};

std::function<double (double)>
cfl::Grid::step (double dStepQuality, unsigned iUniformSteps)
{
  return [dStepQuality, iUniformSteps] (double dVar) {
    ASSERT (dVar > VAR_EPS);

    double dH1 = 1. / dStepQuality;
    double dH2 = std::sqrt (1.5 * dVar / double (iUniformSteps));

    return std::min (dH1, dH2);
  };
}

std::function<unsigned (double)>
cfl::Grid::size ()
{
  return [] (double dSize) {
    unsigned iSize = std::ceil (dSize);

    POSTCONDITION (iSize >= dSize);

    return iSize;
  };
}

std::function<unsigned (double)>
cfl::Grid::size2 ()
{
  return [] (double dSize) {
    unsigned iLog2Size = std::ceil (std::log2 (dSize));
    unsigned iSize = std::pow (2, iLog2Size);

    POSTCONDITION (iSize >= dSize);

    return iSize;
  };
}
