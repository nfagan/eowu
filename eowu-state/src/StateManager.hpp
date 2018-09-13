//
//  StateRunner.hpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace eowu {
  class StateManager;
  
  class State;
}

class eowu::StateManager {
public:
  StateManager() = default;
  ~StateManager();
  
  eowu::State* CreateState(const std::string &id);
  eowu::State* GetState(const std::string &id) const;
  bool HasState(const std::string &id) const;
  std::vector<std::string> GetStateIds() const;
  
private:
  mutable std::shared_mutex mut;
  std::unordered_map<std::string, eowu::State*> states;
};

