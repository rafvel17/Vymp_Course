#include "i-flight-task.h"
#include "i-strike-service.h"
#include "logger.h"
#include "parabola.h"
#include "t-element.h"
#include "auxiliary.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <stdlib.h>
#include <thread>
#include <vector>

using namespace logging;
int N = 50; // количество БР в ударе
double S = 100; // максимальное удаление точки старта от [0;0]
double R = 500; // максимальная дальность полёта
double H = 400; // максимальная высота полёта
double T = 100; // максимальная задержка времени старта
std::vector<std::shared_ptr<tElement> >
GetBMs ()
{
  std::vector<std::shared_ptr<tElement> > strike;
  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen (rd ()); // Standard mersenne_twister_engine seeded with
                            // rd()
  std::uniform_real_distribution<> distrSx (0, S);
  std::uniform_real_distribution<> distrRx (0, R);
  std::uniform_real_distribution<> distrT (0, T);
  std::uniform_int_distribution<> distrN (0, N);
  auto n = distrN (gen);
  for (size_t i = 0; i < n; i++)
    {
      auto sx = distrSx (gen);
      auto rx = distrRx (gen);
      auto syMax = sqrt (S * S - sx * sx);
      auto ryMax = sqrt (R * R - rx * rx);
      std::uniform_real_distribution<> distrSy (0, syMax);
      std::uniform_real_distribution<> distrRy (0, std::min(ryMax, H-syMax));
      auto sy = distrSy (gen);
      auto ry = distrRy (gen);
      tPoint start (sx, sy);
      tPoint finish (sx + rx, sy + ry);
      std::uniform_real_distribution<> distrH (std::max (start.y, finish.y), H);
      auto h = distrH (gen);
      auto t0 = distrT (gen);
      std::shared_ptr<IFlightTask> model (new Parabola (start, finish, h));
      std::shared_ptr<tElement> element (new tElement (model, t0));
      strike.push_back (element);
    }
  // std::uniform_int_distribution<> distrSy(0, )
  return strike;
}
/**
size_t
GetMaxBMsInFlight (const std::vector<std::shared_ptr<tElement> > &p_strike)
{
  std::vector<std::pair<double, double>> ranges;
  for (const auto& el: p_strike)
    ranges.push_back({el->Launch(), el->Boom()});
  
}
**/
void
AddElements ()
{
  IStrikeService &service = IStrikeService::GetInstance ();
  auto strike = GetBMs ();
  for (size_t i = 0; i < strike.size (); i++)
    {
      auto model = strike.at(i)->m_model;
      auto id = service.AddStrikeElement (strike.at (i));
      if(strike.at(i)->isValid())
        INFO ("General", "Rocket " + std::to_string (id) + " T = "
                            + std::to_string (strike.at (i)->Boom ()
                                              - strike.at (i)->Launch ()));
      else
        ERROR("General", "Rocket " + std::to_string(id) + "\n" + GetModelErrorMessage(model));
    }
}

int
main ()
{
  // IStrikeService& service = IStrikeService::GetInstance();
  std::thread th1(AddElements);
  th1.join();
}