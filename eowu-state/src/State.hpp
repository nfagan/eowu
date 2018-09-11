//
//  State.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <eowu-common/Timer.hpp>
#include <functional>
#include <mutex>
#include <string>
#include <vector>

namespace eowu {
  class State;
  class StateManager;
  
  using StateCallbackType = std::function<void(eowu::State*)>;
  using ExitConditionType = std::function<bool(const eowu::State*)>;
}

class eowu::State {
  
public:
  State(const eowu::StateManager *manager, const std::string &id);
  State(const eowu::State &other);
  
  ~State() = default;
  
  void AddExitCondition(const eowu::ExitConditionType &condition);
  
  void SetDuration(eowu::time::DurationType duration);
  void SetOnEntry(const eowu::StateCallbackType &cb);
  void SetOnLoop(const eowu::StateCallbackType &cb);
  void SetOnExit(const eowu::StateCallbackType &cb);
  
  bool ShouldExit() const;
  
  void OnEntry();
  void OnExit();
  void OnLoop();
  
  void Exit(eowu::State *state);
  void Exit();
  void Next(eowu::State *state);
  
  const std::string& GetId() const;
  eowu::State* GetState(const std::string &id) const;
  eowu::State* GetNext();
  const eowu::Timer& GetTimer() const;
  
private:
  mutable std::mutex mut;
  
  eowu::Timer timer;
  
  const eowu::StateManager *manager;
  std::string id;
  eowu::State* next_state;
  
  bool called_next;
  
  std::vector<eowu::ExitConditionType> persistent_exit_conditions;
  
  StateCallbackType on_entry;
  StateCallbackType on_loop;
  StateCallbackType on_exit;
  StateCallbackType on_next;
  
  void entry();
  void loop();
  bool check_exit_conditions() const;
  void set_default_callbacks();
};
