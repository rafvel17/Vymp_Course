#include "i-strike-service.h"
#include <algorithm>
#include <cassert>
 
long
IStrikeService::AddStrikeElement (std::shared_ptr<tElement> element)
{
  locker.lock();
  size_t currNumber = std::max_element(m_elements.begin(), m_elements.end(), [](auto a, auto b) { return a.first < b.first;})->first + 1;
  m_elements[currNumber] = element;
  m_elementsStatus[currNumber] = element->isValid() ? EStatus::READY : EStatus::POOR_INPUT;
  locker.unlock();
  return currNumber;
}
IStrikeServiceInterface::EStatus
IStrikeService::GetStatus(long p_id)
{
  return m_elementsStatus.at(p_id); 
}
void 
IStrikeService::MoveElement(long p_id, double t) 
{
  locker.lock();
  assert(m_elementsStatus.at(p_id) != EStatus::POOR_INPUT);
  m_elementsStatus.at(p_id) = EStatus::IN_PROGRESS;
  locker.unlock();
  m_elements.at(p_id)->Move(t);
}
const tPoint &IStrikeService::Position (long p_id) const
{
  return m_elements.at(p_id)->Position();
}