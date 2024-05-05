// do not include this file

inline cfl::InterestRateModel
cfl::HullWhite::model (const Data &rData, double dInterval,
                       double dStepQuality, double dWidthQuality,
                       unsigned iUniformSteps)
{
  return cfl::HullWhite::model (
      rData, dInterval, brownian (dStepQuality, dWidthQuality, iUniformSteps));
}
