#ifndef __cflInterestRateModel_hpp__
#define __cflInterestRateModel_hpp__

/**
 * @file InterestRateModel.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief Financial models for interest rates.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cfl/Slice.hpp>

namespace cfl
{
/**
 * @ingroup cflModel
 * @defgroup cflInterestRateModel Interest rate models.
 * This module deals with interest rate models.
 * @{
 */

/**
 * @brief  The interface class for an interest rate model.
 *
 * This is the abstract class for an interest rate models. It is used
 * to implement the class InterestRateModel.
 *
 * @see InterestRateModel
 */
class IInterestRateModel
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IInterestRateModel (){};

  /**
   * The virtual constructor. Constructs a new implementation of the same model
   * but with a different vector of event times.
   *
   * @param rEventTimes A new vector of event times for the model.
   * The first element has to coincide with the initial time.
   * @return A new  implementation of the interface class
   * IInterestRateModel with the vector of event times \p rEventTimes.
   */
  virtual IInterestRateModel *
  newModel (const std::vector<double> &rEventTimes) const
      = 0;

  /**
   * A constant reference to the underlying implementation of IModel.
   *
   * @return A constant reference to the underlying implementation of
   * IModel.
   */
  virtual const IModel &model () const = 0;

  /**
   * Constructs the discount factor with maturity \p dBondMaturity at
   * event time with index \p iEventTime.
   * @param iEventTime The index of event time where the discount factor is
   * constructed.
   * @param dBondMaturity The maturity of the discount factor.
   * @return The discount factor with maturity \p dBondMaturity at
   * event time with index \p iEventTime.
   */
  virtual Slice discount (unsigned iEventTime, double dBondMaturity) const = 0;
};

/**
 * @brief  The concrete class for interest rate models.
 *
 * This is the main concrete class for interest rate models.
 *
 * @see IInterestRateModel
 */
class InterestRateModel
{
public:
  /**
   * The constructor.
   *
   * @param pNewModel A pointer to new implementation of
   * IInterestRateModel.
   */
  InterestRateModel (IInterestRateModel *pNewModel);

  /**
   * Resets the vector of event times to \p rEventTimes and removes
   * all path dependent state processes. After this operation, the member
   * function \p model() will return a reference to a different "standard"
   * implementation of IModel.
   *
   * @param rEventTimes The new vector of event times for the model. The first
   * element of this vector equals the initial time of the model.
   */
  void assignEventTimes (const std::vector<double> &rEventTimes);

  /**
   * @copydoc IInterestRateModel::model
   */
  const IModel &model () const;

  /**
   * Returns the vector of event times in the model.
   * The same as <code>model().eventTimes()</code>.
   */
  const std::vector<double> &eventTimes () const;

  /**
   * Returns the initial time of the model.
   * The same as <code>model().eventTimes().front()</code>
   *
   * @return The initial time of the model.
   */
  double initialTime () const;

  /**
   * Constructs the constant payoff in the amount \p dAmount at the
   * event time with index \p iEventTime.
   *
   * @param iEventTime The index of event time.
   * @param dAmount The amount of cash.
   * @return The representation of the constant payoff in the amount
   * \p dAmount taking place at the event time with index \p
   * iEventTime.
   */
  Slice cash (unsigned iEventTime, double dAmount) const;

  /**
   * @copydoc IInterestRateModel::discount
   */
  Slice discount (unsigned iEventTime, double dBondMaturity) const;

  /**
   * Returns the value of state process with index \a iState
   * at the event time with index \a iEventTime.
   * The same as <code>model().state(iEventTime, iState)</code>
   */
  Slice state (unsigned iEventTime, unsigned iState) const;

private:
  std::shared_ptr<IInterestRateModel> m_pModel;
};
/** @} */
} // namespace cfl

#include "cfl/Inline/iInterestRateModel.hpp"
#endif // of __cflInterestRateModel_hpp__
