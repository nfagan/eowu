//
//  EventBus.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/10/18.
//

#include "EventBus.hpp"

eowu::u64 eowu::EventBus::Subscribe(const std::string &event_name, const eowu::EventCallbackType &func) {
  std::unique_lock<std::mutex> lock(mut);
  
  if (HasEvent(event_name)) {
    auto &events_this_name = events.at(event_name);
    auto &event_ids_this_name = event_ids.at(event_name);
    
    eowu::u64 current_size = events_this_name.size();
    
    events_this_name.push_back(func);
    //
    
  } else {
    //
  }
  
  return 0;
}

eowu::u64 eowu::EventBus::get_next_id() const {
  static eowu::u64 id = 0;
  
  return id++;
}

bool eowu::EventBus::HasEvent(const std::string &name) const {
  return events.count(name) > 0;
}
