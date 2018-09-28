//
//  StateRunner.hpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <eowu-common/Timer.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <atomic>

namespace eowu {
  class StateRunner;
  class State;
}

class eowu::StateRunner {
public:
  StateRunner();
  ~StateRunner() = default;
  
  void Begin(eowu::State *state);
  void Exit();
  bool Update();
  
  bool IsNewState() const;
  bool ActiveStateWillExit() const;
  
  const eowu::Timer& GetTimer() const;
private:
  eowu::State *active_state;
  bool is_new_state;
  eowu::Timer timer;
  
  std::atomic<bool> runner_should_exit;
  
  void next(eowu::State *state);
};
