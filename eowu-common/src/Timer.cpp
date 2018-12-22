//
//  Timer.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include <eowu-common/Timer.hpp>

eowu::Timer::Timer() : last_time(eowu::time::now()) {
  //
}

eowu::Timer::Timer(const eowu::Timer &other) : last_time(other.last_time.load()) {
  //
}

void eowu::Timer::Reset() {
  last_time = eowu::time::now();
}

eowu::time::DurationType eowu::Timer::Elapsed() const {
  return eowu::time::now() - last_time.load();
}
