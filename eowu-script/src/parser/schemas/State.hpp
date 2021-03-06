//
//  State.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include "Lua.hpp"
#include <eowu-data.hpp>
#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    using RefFunctionAggregateType = std::unordered_map<std::string, luabridge::LuaRef>;
    
    struct State {
      std::unordered_map<std::string, eowu::data::Commitable> variables;
      
      luabridge::LuaRef entry_function;
      luabridge::LuaRef loop_function;
      luabridge::LuaRef exit_function;
      
      eowu::schema::RefFunctionAggregateType render_functions;
      eowu::schema::RefFunctionAggregateType flip_functions;
      
      std::string state_id;
      
      bool is_first;
      double duration;
      
      State(lua_State *L);
      ~State() = default;
    };
    
    struct States {
      std::unordered_map<std::string, eowu::schema::State> mapping;
    };
  }
}
