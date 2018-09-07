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

namespace eowu {
  class StateRunner;
  class State;
}

class eowu::StateRunner {
public:
  StateRunner();
  ~StateRunner() = default;
  
  void Begin(eowu::State *state);
  bool Update();
  bool IsNewState() const;
  
private:
  eowu::State *active_state;
  bool is_new_state;
  Timer timer;
  
  void next(eowu::State *state);
};
