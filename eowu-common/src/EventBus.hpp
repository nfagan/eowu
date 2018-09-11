//
//  EventBus.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/10/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <functional>
#include <mutex>
#include <vector>
#include <string>
#include <unordered_map>

namespace eowu {
  class EventBus;
  
  using EventCallbackType = std::function<void(void)>;
}

class eowu::EventBus {
  
public:
  
  eowu::u64 Subscribe(const std::string &event_name, const eowu::EventCallbackType &func);
  void Unsubscribe(eowu::u64 id);
  void Emit(const std::string &event_name);
  bool HasEvent(const std::string &name) const;
  
  static eowu::EventBus* GetInstance();
private:
  
  EventBus() = default;
  ~EventBus() = default;
  
  mutable std::mutex mut;
  
  std::unordered_map<std::string, std::unordered_map<eowu::u64, eowu::u64>> event_ids;
  std::unordered_map<std::string, std::vector<eowu::EventCallbackType>> events;
  
  eowu::u64 get_next_id() const;
};
