#pragma once
#include "t-point.h"
class IFlightTask
{
public:
  IFlightTask(const tPoint &p_from, const tPoint &p_to, double p_maxH): 
    startPos(p_from), finishPos(p_to), maxHeight(p_maxH){};
  virtual ~IFlightTask() {};
  virtual double FlightT () const = 0;
  virtual const tPoint &Position () const = 0;
  virtual void Move (double t) = 0;
  virtual bool IsValid() const = 0;
public:
  const tPoint startPos;
  const tPoint finishPos;
  const double maxHeight;
};
