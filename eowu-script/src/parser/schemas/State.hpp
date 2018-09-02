//
//  State.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>
#include "Lua.hpp"

namespace eowu {
  namespace schema {
    struct State {
      luabridge::LuaRef entry_function;
      luabridge::LuaRef loop_function;
      luabridge::LuaRef exit_function;
    };
  }
}
