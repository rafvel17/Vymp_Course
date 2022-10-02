#include "t-point.h"
#include <cassert>
tPoint::tPoint (double p_x, double p_y) : x (p_x), y (p_y) {}
tPoint
tPoint::operator- () const
{
  tPoint tmp;
  tmp.x = -x;
  tmp.y = -y;
  return tmp;
}

tPoint
operator+ (const tPoint &lhs, const tPoint &rhs)
{
  return tPoint (lhs.x + rhs.x, lhs.y + rhs.y);
}

tPoint
operator- (const tPoint &lhs, const tPoint &rhs)
{
  return lhs + (-rhs);
}

bool
operator== (const tPoint &lhs, const tPoint &rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

tPoint
operator* (double lhs, const tPoint &rhs)
{
  return tPoint (rhs.x * lhs, rhs.y * lhs);
}

tPoint
operator* (const tPoint &lhs, double rhs)
{
  return tPoint (lhs.x * rhs, lhs.y * rhs);
}

tPoint
operator/ (const tPoint &lhs, double rhs)
{
  return lhs * (1/rhs);
}

std::ostream& operator<<(std::ostream& out, const tPoint& point)
{
  assert(out);
  out << "(" << point.x << ", " << point.y << ")";
  return out;
}