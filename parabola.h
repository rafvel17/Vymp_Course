#pragma once
#include "t-element.h"
#include "i-flight-task.h"
#ifndef G
#define G 9.81
#endif
class Parabola : public IFlightTask
{
public:
  static double GetTFlight(const tPoint &p_from, const tPoint &p_to, double p_maxH);

  static double GetV0y(const tPoint &p_from, const tPoint &p_to, double p_maxH);

  static double GetV0x(const tPoint &p_from, const tPoint &p_to, double p_maxH);

  static tPoint GetV0(const tPoint &p_from, const tPoint &p_to, double p_maxH);
  
public:
  Parabola (const tPoint &p_from, const tPoint &p_to, double p_maxH);

  double FlightT () const override;

  const tPoint &Position () const override;

  void Move (double t) override;

  bool IsValid() const override;
private:
  const double tFlight;
  
  const tPoint v0;

  const tPoint a0;
  
  const bool isValid;

  double m_currTime;

  tPoint m_currPos;
};