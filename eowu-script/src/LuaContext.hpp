//
//  LuaContext.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <mutex>

struct lua_State;

namespace eowu {
  class LuaContext;
  class LuaFunction;
}

class eowu::LuaContext {
  
public:
  LuaContext();
  LuaContext(lua_State *L);
  ~LuaContext() = default;
  
  lua_State* GetState() const;
  void SetState(lua_State *L);
  
  void Call(const eowu::LuaFunction &other) const;
private:
  mutable std::mutex mut;
  
  lua_State *lua_state;
};
