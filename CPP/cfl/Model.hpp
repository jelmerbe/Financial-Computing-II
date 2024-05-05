#ifndef __cfl_Model_hpp__
#define __cfl_Model_hpp__

/**
 * @file Model.hpp
 * @author Dmitry Kramkov (kramkov@andrew.cmu.edu)
 * @brief The interface class for financial models.
 * @version 1.0
 * @date 2021-01-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "cfl/MultiFunction.hpp"

namespace cfl
{
class Slice;

/**
 * \addtogroup cflBasicElements
 * @{
 */

/**
 * @brief  The interface class for financial models.
 *
 * The interface class IModel defines the model-specific behavior of
 * a Slice object.
 *
 * @see Slice, Model
 */
class IModel
{
public:
  /**
   * The virtual destructor.
   */
  virtual ~IModel () {}

  /**
   * Returns a constant reference to the vector of event times in
   * the model. Event times are sorted in a strictly increasing order
   * and are given as year fractions. The first event time coincides
   * with the initial time.
   *
   * @return A constant reference to the vector of event times in
   * the model.
   */
  virtual const std::vector<double> &eventTimes () const = 0;

  /**
   * Returns the dimension of the model, that is, the number of
   * state processes.
   *
   * @return The number of state processes in the model.
   */
  virtual unsigned numberOfStates () const = 0;

  /**
   * Returns the size of a Slice object that is defined at event
   * time with index \p iEventTime and depends on the state
   * processes with indexes \p rStates.
   *
   * @param iEventTime The index of event time.
   * @param rStates The indexes of state processes.
   * @return The number of nodes in a Slice object defined at the
   * event time with index \p iEventTime and dependent on the state
   * processes with indexes \p rStates.
   */
  virtual unsigned
  numberOfNodes (unsigned iEventTime,
                 const std::vector<unsigned> &rStates) const = 0;

  /**
   * Returns the state process with index \p iState at the event
   * time with index \p iEventTime.
   *
   * @param iEventTime The index of the event time.
   * @param iState The index of the state process.
   * @return The state process with index \p iState at event time
   * with index \p iEventTime.
   */
  virtual Slice state (unsigned iEventTime, unsigned iState) const = 0;

  /**
   * Returns the vector of initial values of the state processes. The
   * size of this vector is the same as <code>numberOfStates()</code>.
   *
   * @return The vector of initial values of the state processes.
   */
  virtual std::valarray<double> origin () const = 0;

  /**
   * Transforms \p rSlice into the equivalent Slice object that also
   * depends on the state processes with indexes \p rStates.  This
   * function is used to define arithmetic operations between Slices
   * dependent on different state processes.
   *
   * @param rSlice The representation of some random variable in the model.
   * After this operation, the random variable will also be dependent on the
   * state processes with indexes \p rStates.
   * @param rStates Additional indexes of state processes.
   */
  virtual void addDependence (Slice &rSlice,
                              const std::vector<unsigned> &rStates) const = 0;

  /**
   * "Rolls back"  \p rSlice to the event time with index \p iEventTime.
   *
   * @param rSlice Before the rollback operator, this object
   * represents the payoff of a financial security at an event time
   * whose index is larger than \p iEventTime. After the rollback
   * operator, it defines the value of this payoff at the event time
   * with index \p iEventTime.
   * @param iEventTime The index of the target event time for \p rSlice.
   */
  virtual void rollback (Slice &rSlice, unsigned iEventTime) const = 0;

  /**
   * Transforms \p rSlice into the indicator function of the event:
   * <code>rSlice >= dBarrier</code>.
   *
   * @param rSlice Before the operation, \p rSlice represents some
   * random variable \p X.  After the operation, \p rSlice becomes the
   * indicator of the event: <code>X >= dBarrier</code>.
   * @param dBarrier The value of the barrier.
   */
  virtual void indicator (Slice &rSlice, double dBarrier) const = 0;

  /**
   * The first component of this multifunction explicitly defines the
   * dependence of a given Slice object on the state processes. Other
   * components may return values of various sensitivity parameters of
   * \p rSlice such as delta and gamma. The domain dimension of the
   * returned MultiFunction object coincides with the number of state
   * processes on which \p rSlice is dependent.
   *
   * @param rSlice A random variable in the model.
   * @return Multi-dimensional function object that shows the
   * dependence of a given Slice object on the state processes in the
   * model. The first component contains the value of the
   * function. Other components describe sensitivities such as delta
   * and gamma.
   */
  virtual MultiFunction interpolate (const Slice &rSlice) const = 0;
};

/**
 * @brief  The concrete class for financial models.
 *
 * This is the standard concrete class for the financial model. It is
 * implemented by a new implementation of interface class IModel.
 *
 * @see IModel
 */
class Model
{
public:
  /**
   * Constructs \p *this from new implementation of IModel. This is the main
   * constructor for Model.
   *
   *@param pNewP A pointer to new implementation of IModel.
   */
  explicit Model (IModel *pNewP = 0);

  /**
   * @copydoc InterestRateModel::eventTimes
   */
  const std::vector<double> &eventTimes () const;

  /**
   * @copydoc InterestRateModel::state
   *
   */
  Slice state (unsigned iEventTime, unsigned iState) const;

  /**
   * @copydoc InterestRateModel::model
   */
  const IModel &model () const;

private:
  std::shared_ptr<IModel> m_pModel;
};
/** @} */
} // namespace cfl

#include "cfl/Inline/iModel.hpp"
#endif // of __cfl_Model_hpp__
