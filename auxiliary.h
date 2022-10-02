#ifndef __AUXILIARY_H__
#define __AUXILIARY_H__
#include "i-flight-task.h"
#include <string>
#include <memory>
#include <sstream>
std::string GetModelErrorMessage(std::shared_ptr<IFlightTask> model)
{
  std::stringstream ss;
  ss << "Wrong input data" << std::endl 
     << "StartPos = " << model->startPos << "\t"
     << "FinishPos = " << model->finishPos << "\t"
     << "MaxHeight = " << model->maxHeight;
  return ss.str();
}
// std::string getModelInfoMessage(std::shared_ptr<IFlightTask> model)
// {

// }
#endif