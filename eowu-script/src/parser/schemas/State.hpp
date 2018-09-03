//
//  State.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include "Lua.hpp"
#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    struct State {
      luabridge::LuaRef entry_function;
      luabridge::LuaRef loop_function;
      luabridge::LuaRef exit_function;
      luabridge::LuaRef render_function;
      
      double duration;
      
      State(lua_State *L);
      ~State() = default;
    };
    
    struct States {
      std::unordered_map<std::string, State> mapping;
    };
  }
}
