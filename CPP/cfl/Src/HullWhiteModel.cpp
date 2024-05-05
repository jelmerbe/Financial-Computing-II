#include "cfl/HullWhiteModel.hpp"
#include "cfl/Data.hpp"
#include "cfl/Error.hpp"
#include "cfl/Similar.hpp"
#include <limits>

using namespace cfl::HullWhite;
using namespace cfl;

// class HullWhite::Data

cfl::HullWhite::Data
cfl::HullWhite::makeData (const Function &rDiscount,
                          const Function &rVolatility, const Function &rShape,
                          double dInitialTime)
{
  PRECONDITION (std::abs (rShape (dInitialTime)) < cfl::EPS);

  cfl::HullWhite::Data uData;
  uData.discount = rDiscount;
  uData.shape = rShape;
  uData.volatility = rVolatility;
  uData.initialTime = dInitialTime;

  return uData;
};

Function
bondShape (double dLambda, double dInitialTime)
{
  std::function<double (double)> uShape
      = [dLambda, dInitialTime] (double dTime) {
          PRECONDITION (dTime >= dInitialTime);

          double dT = dTime - dInitialTime;
          return (std::abs (dLambda) <= cfl::EPS)
                     ? dT
                     : (1. - std::exp (-dLambda * dT)) / dLambda;
        };

  return Function (uShape, dInitialTime);
}

cfl::HullWhite::Data
cfl::HullWhite::makeData (const Function &rDiscount, double dSigma,
                          double dLambda, double dInitialTime)
{
  Function uVolatility = cfl::Data::volatility (dSigma, dLambda, dInitialTime);
  Function uShape = bondShape (dLambda, dInitialTime);

  return makeData (rDiscount, uVolatility, uShape, dInitialTime);
}

// construction of Hull and White model
namespace cflHullWhite
{
Slice
discount (unsigned iTime, double dMaturity, const cfl::HullWhite::Data &rData,
          const IModel &rModel)
{
  PRECONDITION (iTime < rModel.eventTimes ().size ());
  PRECONDITION (dMaturity >= rModel.eventTimes ()[iTime]);

  double dRefTime = rModel.eventTimes ()[iTime];
  double dA = rData.shape (dRefTime);
  double dB = rData.shape (dMaturity);
  double dC = rData.shape (rModel.eventTimes ().back ());
  double dVar = std::pow (rData.volatility (dRefTime), 2)
                * (dRefTime - rData.initialTime);
  double dForwardDiscount
      = rData.discount (dMaturity) / rData.discount (dRefTime);
  Slice uDiscount = exp (rModel.state (iTime, 0) * (dB - dA));
  uDiscount *= (dForwardDiscount
                * std::exp (-0.5 * (dB - dA) * (dA + dB - 2. * dC) * dVar));
  return uDiscount;
}

TRollback
rollback (const IModel &rModel, const HullWhite::Data &rData)
{
  return [&rModel, &rData] (Slice &rSlice, unsigned iTime) {
    PRECONDITION (rSlice.timeIndex () >= iTime);
    PRECONDITION (&rSlice.model () == &rModel);

    double dMaturity = rModel.eventTimes ().back ();
    rSlice /= discount (rSlice.timeIndex (), dMaturity, rData, rModel);
    rSlice.rollback (iTime);
    rSlice *= discount (iTime, dMaturity, rData, rModel);
  };
}

class Model : public IInterestRateModel
{
public:
  Model (const HullWhite::Data &rData, const std::vector<double> &rEventTimes,
         double dInterval, const TBrownian &rBrownian)
      : m_uData (rData), m_dInterval (dInterval), m_uBrownian (rBrownian)
  {
    PRECONDITION (rEventTimes.front () == rData.initialTime);

    std::vector<double> uVar (rEventTimes.size ());
    std::transform (rEventTimes.begin (), rEventTimes.end (), uVar.begin (),
                    [&rData] (double dTime) {
                      return std::pow (rData.volatility (dTime), 2);
                    });
    cfl::Model uBrownian = m_uBrownian (uVar, rEventTimes, dInterval);
    TRollback uRollback = rollback (uBrownian.model (), m_uData);
    m_uModel = similar (uRollback, uBrownian);
  }

  IInterestRateModel *
  newModel (const std::vector<double> &rEventTimes) const
  {
    return new Model (m_uData, rEventTimes, m_dInterval, m_uBrownian);
  }

  const IModel &
  model () const
  {
    return m_uModel.model ();
  }

  Slice
  discount (unsigned iTime, double dMaturity) const
  {
    double dTime = model ().eventTimes ()[iTime];

    ASSERT (dTime <= dMaturity);

    if (dTime == dMaturity)
      {
        return Slice (&model (), iTime, 1.);
      }

    return cflHullWhite::discount (iTime, dMaturity, m_uData, model ());
  }

private:
  HullWhite::Data m_uData;
  double m_dInterval;
  TBrownian m_uBrownian;
  cfl::Model m_uModel;
};
} // namespace cflHullWhite

// function cfl::HullWhite::model
InterestRateModel
cfl::HullWhite::model (const HullWhite::Data &rData, double dInterval,
                       const TBrownian &rBrownian)
{
  std::vector<double> uEventTimes (1, rData.initialTime);

  return InterestRateModel (
      new cflHullWhite::Model (rData, uEventTimes, dInterval, rBrownian));
}
