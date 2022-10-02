#include "i-flight-task.h"
#include "t-element.h"

tElement::tElement (std::shared_ptr<IFlightTask> model, double p_t0)
    : t0 (p_t0), m_model (model)
{
}



bool
tElement::isValid () const
{
  return m_model->IsValid();
}

void
tElement::Move (double t)
{
  if (t < t0)
    return;
  m_locker.lock();
  m_model->Move (t - t0);
  m_locker.unlock();
}