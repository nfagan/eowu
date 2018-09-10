//
//  TaskTiming.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <atomic>

namespace eowu {
  namespace time {
    class TaskTiming;
  }
}

class eowu::time::TaskTiming {
public:
  TaskTiming() = default;
  ~TaskTiming() = default;
  
  void Update(double current_time_seconds);
  
  eowu::u64 GetIteration() const;
  double GetCurrentTime() const;
private:
  std::atomic<eowu::u64> iteration;
  std::atomic<double> current_time_seconds;
};
