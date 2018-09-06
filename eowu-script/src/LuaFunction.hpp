//
//  LuaFunction.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <Lua.hpp>
#include <mutex>

namespace eowu {
  class LuaFunction;
}

class eowu::LuaFunction {
public:
  LuaFunction(const luabridge::LuaRef &ref);
  LuaFunction(const eowu::LuaFunction &other);
  
  void Set(const eowu::LuaFunction &other);
  void Set(const luabridge::LuaRef &func);
  void Call() const;
  
  static const LuaFunction& get_no_op(lua_State *L);
private:
  mutable std::recursive_mutex mut;
  
  luabridge::LuaRef function_reference;
};
