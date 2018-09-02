//
//  ExitConditions.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "ExitConditions.hpp"
#include "State.hpp"

bool eowu::exit_conditions::time_exceeded(const State *state, eowu::time::DurationType duration) {
  return state->GetTimer().Ellapsed() > duration;
}
