#ifndef __cflAssetModel_hpp__
#define __cflAssetModel_hpp__

/**
 * @file  AssetModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Financial model for a single asset.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/InterestRateModel.hpp"

namespace cfl
{
/**
 * @defgroup cflModel Financial models.
 *
 * This module contains implementations of financial models.
 *
 */

/**
 * @ingroup cflModel
 *
 * @defgroup cflAssetModel Single asset models.
 *
 * This module deals with financial models for a single asset.
 * @{
 */

/**
 * @brief  The interface class for a single asset model.
 *
 * This is the interface class for a single asset model. It is used
 * to implement AssetModel.
 *
 * @see AssetModel
 */
class IAssetModel
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IAssetModel (){};

  /**
   * The virtual constructor. Constructs new implementation of the same model
   * but with a different vector of event times.
   *
   * @param rEventTimes New vector of event times for the model.
   * The first element needs to coincide with the initial time.
   * @return Pointer to a new implementation of IAssetModel which has
   * event times \p rEventTimes.
   */
  virtual IAssetModel *
  newModel (const std::vector<double> &rEventTimes) const = 0;

  /**
   * @copydoc IInterestRateModel::model
   */
  virtual const IModel &model () const = 0;

  /**
   * @copydoc IInterestRateModel::discount
   */
  virtual Slice discount (unsigned iEventTime, double dBondMaturity) const = 0;

  /**
   * Returns the forward price for the delivery time \p dForwardMaturity
   * at the event time with index \p iEventTime.
   *
   * @param iEventTime The index of an event time.
   * @param dForwardMaturity The maturity of the forward contract.
   * @return The forward price of underlying asset for the contract with
   * delivery time \p dForwardMaturity at the event time with index \p
   * iEventTime.
   */
  virtual Slice forward (unsigned iEventTime,
                         double dForwardMaturity) const = 0;
};

/**
 * @brief  The concrete class for financial models with a single asset.
 *
 * This is the universal class for financial models with a single asset.
 * It is constructed from a new implementation of IAssetModel.
 */
class AssetModel
{
public:
  /**
   * The constructor.
   *
   * @param pNewModel A pointer to new implementation of IAssetModel.
   */
  AssetModel (IAssetModel *pNewModel);

  /**
   * @copydoc InterestRateModel::assignEventTimes
   */
  void assignEventTimes (const std::vector<double> &rEventTimes);

  /**
   * @copydoc InterestRateModel::model
   */
  const IModel &model () const;

  /**
   * @copydoc InterestRateModel::eventTimes
   */
  const std::vector<double> &eventTimes () const;

  /**
   * @copydoc InterestRateModel::initialTime
   */
  double initialTime () const;

  /**
   * @copydoc InterestRateModel::cash
   */
  Slice cash (unsigned iEventTime, double dAmount) const;

  /**
   * @copydoc InterestRateModel::discount
   */
  Slice discount (unsigned iEventTime, double dBondMaturity) const;

  /**
   * @copydoc IAssetModel::forward
   */
  Slice forward (unsigned iEventTime, double dForwardMaturity) const;

  /**
   * Returns the spot price at event time with index \p iEventTime.
   *
   * @param iEventTime The index of an event time.
   * @return Spot price of underlying asset at event time with index \p
   * iEventTime.
   */
  Slice spot (unsigned iEventTime) const;

  /**
   * @copydoc InterestRateModel::state
   */
  Slice state (unsigned iEventTime, unsigned iState) const;

private:
  std::shared_ptr<IAssetModel> m_pModel;
};
/** @} */
} // namespace cfl

#include "cfl/Inline/iAssetModel.hpp"
#endif // of __cflAssetModel_hpp__
