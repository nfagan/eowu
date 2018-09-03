//
//  LuaStateManager.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include "Init.hpp"

struct lua_State;

namespace eowu {
  class LuaContext;
  class TaskWrapper;
}

class eowu::TaskWrapper {
public:
  TaskWrapper() = delete;
  
  static eowu::StateWrapper* GetStateWrapper(const std::string &id);
  static void CreateLuaSchema(lua_State *L);
  
  static eowu::StateWrapperContainerType States;
};
