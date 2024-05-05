// do not include this file

inline cfl::AssetModel
cfl::Black::model (const Data &rData, double dInterval, double dStepQuality,
                   double dWidthQuality, unsigned iUniformSteps)
{
  return cfl::Black::model (
      rData, dInterval, brownian (dStepQuality, dWidthQuality, iUniformSteps));
}
