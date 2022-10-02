#ifndef T_ELEMENT_H
#define T_ELEMENT_H
#include "i-flight-task.h"
#include <mutex>
#include <memory>
class tElement
{
protected:
  double t0;
  std::mutex m_locker;
public:
  std::shared_ptr<IFlightTask> m_model;
  tElement (std::shared_ptr<IFlightTask> model, double p_t0 = 0);

  inline double Launch () const {return t0;};

  inline double Boom () const {return t0 + m_model->FlightT();};

  inline const tPoint & Position () const {return m_model->Position ();};

  bool isValid () const;

  virtual void Move (double t);
};


#endif