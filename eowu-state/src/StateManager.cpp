//
//  StateManager.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateManager.hpp"
#include "State.hpp"
#include "Error.hpp"
#include <mutex>

eowu::State* eowu::StateManager::CreateState(const std::string &id) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  if (states.count(id) > 0) {
    throw eowu::StateCreationError("State ids must be unique.");
  }
  
  auto state = new eowu::State(this, id);
  
  states.emplace(id, state);
  
  return state;
}

bool eowu::StateManager::HasState(const std::string &id) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return states.count(id) > 0;
}

eowu::State* eowu::StateManager::GetState(const std::string &id) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  const auto it = states.find(id);
  
  if (it == states.end()) {
    throw eowu::NonexistentStateError("State '" + id + "' does not exist.");
  }
  
  return it->second;
}
