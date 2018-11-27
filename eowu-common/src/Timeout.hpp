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
  enum Type {
    TIMEOUT,
    INTERVAL
  };
  
public:
  Timeout();
  Timeout(const eowu::time::DurationType &duration);
  Timeout(eowu::Timeout::Type type, const eowu::time::DurationType &duration);
  Timeout(const eowu::Timeout &other);
  ~Timeout() = default;
  
  void Update();
  void Reset();
  void Cancel();
  
  eowu::Timeout::Type GetType() const;
  
  void SetCallback(const std::function<void()> &cb);
  
private:
  mutable std::recursive_mutex mut;
  
  eowu::Timeout::Type type;
  
  eowu::Timer timer;
  std::atomic<eowu::time::DurationType> duration;
  std::atomic<bool> is_first_update;
  
  std::function<void()> callback;
  
  void trigger_callback();
  
  static void noop();
};
