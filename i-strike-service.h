#pragma once
#include "i-strike-service-interface.h"
#include "t-element.h"
#include "i-flight-task.h"
#include <map>
#include <mutex>
#include <memory>
class IStrikeService: public IStrikeServiceInterface
{
public:
  static IStrikeService & GetInstance ()
  {
    static IStrikeService s;
    return s;
  }
public:
  long AddStrikeElement (std::shared_ptr<tElement> element) override;
  EStatus GetStatus (long p_id) override;
  void MoveElement (long p_id, double p_t) override;
  const tPoint &Position (long p_id) const override;
private:
  IStrikeService() {};
  std::map<size_t, std::shared_ptr<tElement>> m_elements;
  std::map<size_t, EStatus> m_elementsStatus;
  double m_currentTime;
  std::mutex locker;
};
