//
//  ExitConditions.hpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <eowu-common/time.hpp>

namespace eowu {
  class State;
  
  namespace exit_conditions {
    bool time_exceeded(const State *state, eowu::time::DurationType);
  }
}
