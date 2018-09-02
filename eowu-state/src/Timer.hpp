//
//  Timer.hpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <atomic>
#include <chrono>
#include <eowu-common/time.hpp>

namespace eowu {
  class Timer;
}

class eowu::Timer {
public:
  Timer();
  ~Timer() = default;
  
  void Reset();
  void Update();
  eowu::time::DurationType Ellapsed() const;
  
private:
  std::atomic<eowu::time::TimePointType> current_time;
  std::atomic<eowu::time::TimePointType> last_time;
};