//
//  StateRunner.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateRunner.hpp"
#include "State.hpp"

eowu::StateRunner::StateRunner() : active_state(nullptr), is_new_state(false), runner_should_exit(false) {
  //
}

void eowu::StateRunner::Begin(eowu::State *state) {
  timer.Reset();
  next(state);
}

bool eowu::StateRunner::Update() {  
  eowu::State *current_active_state = active_state.load();
  
  if (current_active_state == nullptr || runner_should_exit) {
    return true;
  }
  
  //  If entering, loop will be called right afterwards.
  //  But if exiting, loop won't be called afterwards.
  if (IsNewState()) {
    current_active_state->OnEntry();
    is_new_state = false;
  }
  
  if (current_active_state->ShouldExit()) {
    current_active_state->OnExit();
    next(current_active_state->GetNext());
  } else {
    current_active_state->OnLoop();
  }
  
  return false;
}

void eowu::StateRunner::Exit() {
  runner_should_exit = true;
}

const eowu::Timer& eowu::StateRunner::GetTimer() const {
  return timer;
}

const eowu::State* eowu::StateRunner::GetActiveState() const {
  return active_state;
}

bool eowu::StateRunner::IsNewState() const {
  return is_new_state;
}

bool eowu::StateRunner::ActiveStateWillExit() const {
  eowu::State *current_active_state = active_state.load();
  
  return current_active_state != nullptr && current_active_state->ShouldExit();
}

void eowu::StateRunner::next(eowu::State *state) {
  this->active_state = state;
  is_new_state = true;
}
