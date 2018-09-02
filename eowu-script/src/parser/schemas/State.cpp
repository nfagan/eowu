//
//  State.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "State.hpp"

eowu::schema::State::State(lua_State *L) : entry_function(L), loop_function(L), exit_function(L) {
  //
}
