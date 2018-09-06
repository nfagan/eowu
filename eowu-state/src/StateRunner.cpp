//
//  StateRunner.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateRunner.hpp"
#include "State.hpp"

eowu::StateRunner::StateRunner() {
  is_new_state = false;
  active_state = nullptr;
}

void eowu::StateRunner::Begin(eowu::State *state) {
  timer.Reset();
  next(state);
}

bool eowu::StateRunner::Update() {
  if (active_state == nullptr) {
    return true;
  }
  
  if (IsNewState()) {
    active_state->OnEntry();
    is_new_state = false;
  }
  
  if (active_state->ShouldExit()) {
    active_state->OnExit();
    next(active_state->GetNext());
  } else {
    active_state->OnLoop();
  }
  
  return false;
}

bool eowu::StateRunner::IsNewState() const {
  return is_new_state;
}

void eowu::StateRunner::next(eowu::State *state) {
  this->active_state = state;
  is_new_state = true;
}
