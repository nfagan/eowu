//
//  TaskTiming.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#include "TaskTiming.hpp"

eowu::u64 eowu::time::TaskTiming::GetIteration() const {
  return iteration;
}

double eowu::time::TaskTiming::GetCurrentTime() const {
  return current_time_seconds;
}

void eowu::time::TaskTiming::Update(double current_time_seconds) {
  iteration++;
  this->current_time_seconds = current_time_seconds;
}
