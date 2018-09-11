//
//  RenderTiming.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#include "RenderTiming.hpp"
#include <mutex>

eowu::time::RenderTiming::RenderTiming(const eowu::time::RenderTiming &other) :
mut(), iteration(other.iteration.load()), current_time_seconds(other.current_time_seconds) {
  //
}

eowu::u64 eowu::time::RenderTiming::GetIteration() const {
  return iteration.load();
}

double eowu::time::RenderTiming::GetCurrentTime(const std::string &for_window) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return current_time_seconds.at(for_window);
}

void eowu::time::RenderTiming::IncrementIteration() {
  iteration++;
}

void eowu::time::RenderTiming::SetCurrentTime(const std::string &for_window, double time) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  current_time_seconds[for_window] = time;
}

bool eowu::time::RenderTiming::HasAlias(const std::string &for_window) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return current_time_seconds.count(for_window) > 0;
}

void eowu::time::RenderTiming::UseContents(const eowu::time::RenderTimingCallbackType &cb) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  eowu::u64 iter = iteration.load();
  
  for (const auto &it : current_time_seconds) {
    cb(it.first, iter, it.second);
  }
}
