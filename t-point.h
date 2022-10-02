#pragma once
#include <iostream>
#include <fstream>
struct tPoint
{
  double x, y;
  tPoint (double p_x = 0, double p_y = 0);
  tPoint operator- () const;
};
tPoint operator+ (const tPoint &lhs, const tPoint &rhs);

tPoint operator- (const tPoint &lhs, const tPoint &rhs);

bool operator== (const tPoint &lhs, const tPoint &rhs);

tPoint operator* (double val, const tPoint &point);

tPoint operator* (const tPoint &lhs, double rhs);

tPoint operator/ (const tPoint &lhs, double rhs);

std::ostream& operator<<(std::ostream& out, const tPoint& point); 