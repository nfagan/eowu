//
//  Timeout.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#include "Timeout.hpp"

eowu::Timeout::Timeout() :
duration(eowu::time::zero()), on_ellapsed(&eowu::Timeout::on_ellapsed_noop) {
  //
}

eowu::Timeout::Timeout(const eowu::time::DurationType &dur) :
duration(dur), on_ellapsed(&eowu::Timeout::on_ellapsed_noop) {
  //
}

eowu::Timeout::Timeout(const eowu::Timeout &other) :
timer(other.timer), duration(other.duration.load()) {
  std::unique_lock<std::recursive_mutex> other_lock(other.mut);
  
  on_ellapsed = other.on_ellapsed;
}

void eowu::Timeout::Reset() {
  timer.Reset();
  SetOnEllapsed(&eowu::Timeout::on_ellapsed_noop);
}

void eowu::Timeout::Update() {
  timer.Update();
  
  if (timer.Ellapsed() >= duration.load()) {
    std::unique_lock<std::recursive_mutex> lock(mut);
    
    on_ellapsed();
  }
}

void eowu::Timeout::SetOnEllapsed(const std::function<void()> &cb) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  on_ellapsed = cb;
}

void eowu::Timeout::on_ellapsed_noop() {
  //
}
