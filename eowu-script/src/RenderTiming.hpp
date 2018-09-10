//
//  RenderTiming.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <unordered_map>
#include <atomic>
#include <shared_mutex>
#include <string>
#include <functional>

namespace eowu { 
  namespace time {
    class RenderTiming;
    
    using RenderTimingCallbackType = std::function<void(const std::string&, eowu::u64, double)>;
  }
}

class eowu::time::RenderTiming {
public:
  RenderTiming() = default;
  RenderTiming(const eowu::time::RenderTiming &other);
  ~RenderTiming() = default;
  
  void SetCurrentTime(const std::string &for_window, double time);
  void UseContents(const eowu::time::RenderTimingCallbackType &cb) const;
  bool HasAlias(const std::string &for_window) const;
  
  void IncrementIteration();
  
  eowu::u64 GetIteration() const;
  double GetCurrentTime(const std::string &for_window);
  
private:
  mutable std::shared_mutex mut;
  
  std::atomic<eowu::u64> iteration;
  std::unordered_map<std::string, double> current_time_seconds;
};
