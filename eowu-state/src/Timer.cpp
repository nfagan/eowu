//
//  Timer.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "Timer.hpp"

eowu::Timer::Timer() : current_time(eowu::time::now()), last_time(eowu::time::now()) {
  //
}

eowu::Timer::Timer(const eowu::Timer &other) : current_time(other.last_time.load()), last_time(other.current_time.load()) {
  //
}

void eowu::Timer::Reset() {
  last_time = eowu::time::now();
  current_time = last_time.load();
}

void eowu::Timer::Update() {
  current_time = eowu::time::now();
}

eowu::time::DurationType eowu::Timer::Ellapsed() const {
  return current_time.load() - last_time.load();
}
