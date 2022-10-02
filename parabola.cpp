#include "parabola.h"
#include "t-point.h"
#include <cassert>
#include <cmath>

double
Parabola::GetTFlight (const tPoint &p_from, const tPoint &p_to, double p_maxH)
{
  double v0y = GetV0y (p_from, p_to, p_maxH);
  return v0y / G + sqrt (pow (v0y, 2) - 2 * G * (p_to - p_from).y) / G;
}
double
Parabola::GetV0x (const tPoint &p_from, const tPoint &p_to, double p_maxH)
{
  double t = Parabola::GetTFlight (p_from, p_to, p_maxH);
  return (p_to - p_from).x / t;
}
double
Parabola::GetV0y (const tPoint &p_from, const tPoint &p_to, double p_maxH)
{
  return sqrt (2 * G * (abs (p_maxH - p_from.y)));
}
tPoint
Parabola::GetV0 (const tPoint &p_from, const tPoint &p_to, double p_maxH)
{
  return tPoint (GetV0x (p_from, p_to, p_maxH), GetV0y (p_from, p_to, p_maxH));
}
Parabola::Parabola (const tPoint &p_from, const tPoint &p_to, double p_maxH)
    : IFlightTask(p_from, p_to, p_maxH), tFlight (GetTFlight (p_from, p_to, p_maxH)),
      v0 (GetV0 (p_from, p_to, p_maxH)), a0 (0, -G),
      isValid (p_maxH - p_from.y > 0 && p_maxH - p_to.y > 0)
{
  m_currPos = startPos;
}

double
Parabola::FlightT () const
{
  return tFlight;
}

const tPoint &
Parabola::Position () const
{
  return m_currPos;
}

void
Parabola::Move (double t)
{
  assert (t > m_currTime);
  assert (isValid);
  m_currTime = t;
  m_currPos = startPos + v0 * t + a0 * pow (t, 2) / 2;
}

bool
Parabola::IsValid() const
{
  return isValid;
}