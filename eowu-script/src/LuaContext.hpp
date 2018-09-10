//
//  LuaContext.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <mutex>
#include "LuaFunction.hpp"

struct lua_State;

namespace eowu {
  class LuaContext;
}

class eowu::LuaContext {
  
public:
  LuaContext();
  LuaContext(lua_State *L);
  ~LuaContext() = default;
  
  lua_State* GetState() const;
  void SetState(lua_State *L);
  
  template<typename ...Args>
  void Call(eowu::LuaFunction &other, Args... args) const;
private:
  mutable std::mutex mut;
  
  lua_State *lua_state;
};

#include "LuaContext.hh"
