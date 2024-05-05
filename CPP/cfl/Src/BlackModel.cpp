#include "cfl/BlackModel.hpp"
#include "cfl/Data.hpp"
#include "cfl/Error.hpp"
#include "cfl/Similar.hpp"
#include <limits>

using namespace cfl::Black;
using namespace cfl;

// class Black::Data
cfl::Black::Data
cfl::Black::makeData (const Function &rDiscount, const Function &rForward,
                      const Function &rVolatility, const Function &rShape,
                      double dInitialTime)
{
  PRECONDITION (std::abs (rShape (dInitialTime) - 1.) < cfl::EPS);

  cfl::Black::Data uData;
  uData.discount = rDiscount;
  uData.forward = rForward;
  uData.shape = rShape;
  uData.volatility = rVolatility;
  uData.initialTime = dInitialTime;

  return uData;
};

cfl::Black::Data
cfl::Black::makeData (const Function &rDiscount, const Function &rForward,
                      double dSigma, double dLambda, double dInitialTime)
{
  Function uVolatility = cfl::Data::volatility (dSigma, dLambda, dInitialTime);
  Function uShape = cfl::Data::discount (dLambda, dInitialTime);

  return makeData (rDiscount, rForward, uVolatility, uShape, dInitialTime);
}

cfl::Black::Data
cfl::Black::makeData (const Function &rDiscount, const Function &rForward,
                      const Function &rVolatility, double dInitialTime)
{
  Function uShape (1., dInitialTime);

  return makeData (rDiscount, rForward, rVolatility, uShape, dInitialTime);
}

cfl::Black::Data
cfl::Black::makeData (const Function &rDiscount, const Function &rForward,
                      double dSigma, double dInitialTime)
{
  Function uVolatility (dSigma, dInitialTime);
  Function uShape (1., dInitialTime);

  return makeData (rDiscount, rForward, uVolatility, uShape, dInitialTime);
}

// construction of Black model
namespace cflBlack
{
TRollback
rollback (const IModel &rModel, const Function &rDiscount)
{
  return [&rModel, &rDiscount] (Slice &rSlice, unsigned iTime) {
    PRECONDITION (rSlice.timeIndex () >= iTime);
    PRECONDITION (&rSlice.model () == &rModel);

    double dMaturity = rModel.eventTimes ()[rSlice.timeIndex ()];
    double dToday = rModel.eventTimes ()[iTime];
    double dFactor = rDiscount (dMaturity) / rDiscount (dToday);
    rSlice.rollback (iTime);
    rSlice *= dFactor;
  };
}

class BlackModel : public IAssetModel
{
public:
  BlackModel (const Black::Data &rData, const std::vector<double> &rEventTimes,
              double dInterval, const TBrownian &rBrownian)
      : m_uData (rData), m_dInterval (dInterval), m_uBrownian (rBrownian)
  {
    ASSERT (rEventTimes.front () == rData.initialTime);

    std::vector<double> uVar (rEventTimes.size ());
    std::transform (rEventTimes.begin (), rEventTimes.end (), uVar.begin (),
                    [&rData] (double dTime) {
                      return std::pow (rData.volatility (dTime), 2);
                    });
    cfl::Model uBrownian = m_uBrownian (uVar, rEventTimes, dInterval);
    TRollback uRollback = rollback (uBrownian.model (), m_uData.discount);
    m_uModel = similar (uRollback, uBrownian);
  }

  IAssetModel *
  newModel (const std::vector<double> &rEventTimes) const
  {
    return new BlackModel (m_uData, rEventTimes, m_dInterval, m_uBrownian);
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
    double dFactor = m_uData.discount (dMaturity) / m_uData.discount (dTime);
    return Slice (&model (), iTime, dFactor);
  }

  Slice
  forward (unsigned iTime, double dForwardMaturity) const
  {
    PRECONDITION (iTime < model ().eventTimes ().size ());

    double dRefTime = model ().eventTimes ()[iTime];

    PRECONDITION (dForwardMaturity >= dRefTime);

    // forward price = exp(shape * state + c);
    double dForward = m_uData.forward (dForwardMaturity);
    double dVol = m_uData.volatility (dRefTime);
    double dShape = m_uData.shape (dForwardMaturity);
    double dC = std::log (dForward)
                - 0.5 * std::pow (dVol * dShape, 2)
                      * (dRefTime - m_uData.initialTime);
    Slice uState = model ().state (iTime, 0);

    return exp (uState * dShape + dC);
  }

private:
  Black::Data m_uData;
  double m_dInterval;
  TBrownian m_uBrownian;
  cfl::Model m_uModel;
};
} // namespace cflBlack

// function cfl::Black::model
AssetModel
cfl::Black::model (const Data &rData, double dInterval,
                   const TBrownian &rBrownian)
{
  std::vector<double> uEventTimes (1, rData.initialTime);

  return AssetModel (
      new cflBlack::BlackModel (rData, uEventTimes, dInterval, rBrownian));
}
