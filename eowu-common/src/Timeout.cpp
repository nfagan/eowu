//
//  Timeout.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#include "Timeout.hpp"
#include <iostream>

eowu::Timeout::Timeout() :
type(eowu::Timeout::TIMEOUT),
duration(eowu::time::zero()),
is_first_update(true),
callback(&eowu::Timeout::noop) {
  //
}

eowu::Timeout::Timeout(eowu::Timeout::Type timeout_type,
                       const eowu::time::DurationType &dur) :
type(timeout_type),
duration(dur),
is_first_update(true),
callback(&eowu::Timeout::noop) {
  //
}


eowu::Timeout::Timeout(const eowu::time::DurationType &dur) :
type(eowu::Timeout::TIMEOUT),
duration(dur),
is_first_update(true),
callback(&eowu::Timeout::noop) {
  //
}

eowu::Timeout::Timeout(const eowu::Timeout &other) :
timer(other.timer),
duration(other.duration.load()),
is_first_update(other.is_first_update.load()) {
  std::unique_lock<std::recursive_mutex> other_lock(other.mut);
  
  callback = other.callback;
  type = other.type;
}

void eowu::Timeout::Reset() {
  timer.Reset();
  
  if (type == eowu::Timeout::TIMEOUT) {
    SetCallback(&eowu::Timeout::noop);
  }
}

void eowu::Timeout::Cancel() {
  timer.Reset();
  SetCallback(&eowu::Timeout::noop);
}

void eowu::Timeout::Update() {
  timer.Update();
  
  bool interval_crit = type == eowu::Timeout::INTERVAL && is_first_update.load();
  bool duration_crit = timer.Elapsed() >= duration.load();
  
  if (interval_crit) {
    trigger_callback();
    is_first_update = false;
  } else if (duration_crit) {
    trigger_callback();
  }
}

void eowu::Timeout::trigger_callback() {
  std::unique_lock<std::recursive_mutex> lock(mut);
  callback();
  Reset();
}

eowu::Timeout::Type eowu::Timeout::GetType() const {
  return type;
}

void eowu::Timeout::SetCallback(const std::function<void()> &cb) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  callback = cb;
}

void eowu::Timeout::noop() {
  //
}
