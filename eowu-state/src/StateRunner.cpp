//
//  StateRunner.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateRunner.hpp"
#include "State.hpp"

eowu::StateRunner::StateRunner() : active_state(nullptr), is_new_state(false) {
  //
}

void eowu::StateRunner::Begin(eowu::State *state) {
  timer.Reset();
  next(state);
}

bool eowu::StateRunner::Update() {
  timer.Update();
  
  if (active_state == nullptr) {
    return true;
  }
  
  //  If entering, loop will be called right afterwards.
  //  But if exiting, loop won't be called afterwards.
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

const eowu::Timer& eowu::StateRunner::GetTimer() const {
  return timer;
}

bool eowu::StateRunner::IsNewState() const {
  return is_new_state;
}

bool eowu::StateRunner::ActiveStateWillExit() const {
  return active_state != nullptr && active_state->ShouldExit();
}

void eowu::StateRunner::next(eowu::State *state) {
  this->active_state = state;
  is_new_state = true;
}
