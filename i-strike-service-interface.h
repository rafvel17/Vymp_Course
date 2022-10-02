#pragma once
#include "t-point.h"
#include "t-element.h"
#include <memory>
class IStrikeServiceInterface
{
public:
  enum EStatus : int
  {
    READY = 0,
    IN_PROGRESS,
    POOR_INPUT,
    DONE
  };
 

public:
  static IStrikeServiceInterface& GetInstance();
  virtual ~IStrikeServiceInterface () {};
  virtual long AddStrikeElement (std::shared_ptr<tElement> element) = 0;
  virtual EStatus GetStatus (long p_id) = 0;
  virtual void MoveElement (long p_id, double p_t) = 0;
  virtual const tPoint &Position (long p_id) const = 0;
};