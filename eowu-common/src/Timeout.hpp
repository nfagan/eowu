//
//  Timeout.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#pragma once

#include <eowu-common/Timer.hpp>
#include <mutex>
#include <atomic>
#include <functional>

namespace eowu {
  class Timeout;
}

class eowu::Timeout {
public:
  Timeout();
  Timeout(const eowu::time::DurationType &duration);
  Timeout(const eowu::Timeout &other);
  ~Timeout() = default;
  
  void Update();
  void Reset();
  void SetOnEllapsed(const std::function<void()> &cb);
  bool Ellapsed() const;
  
private:
  mutable std::recursive_mutex mut;
  
  eowu::Timer timer;
  std::atomic<eowu::time::DurationType> duration;
  
  std::function<void()> on_ellapsed;
  
  static void on_ellapsed_noop();
};
