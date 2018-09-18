//
//  LuaFunction.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <Lua.hpp>
#include <mutex>
#include <atomic>

namespace eowu {
  class LuaFunction;
}

class eowu::LuaFunction {
public:
  LuaFunction(lua_State *L);
  LuaFunction(const luabridge::LuaRef &ref);
  LuaFunction(const eowu::LuaFunction &other);
  
  LuaFunction& operator=(const eowu::LuaFunction& other);
  
  void Set(const eowu::LuaFunction &other);
  void Set(const luabridge::LuaRef &func);
  
  bool IsValid() const;
  
  const luabridge::LuaRef& GetReference() const;
  
  void AbortCall();
  bool DidCall() const;
  
  template<typename ...Args>
  void Call(Args... args);
  
  static const LuaFunction& get_no_op(lua_State *L);
private:
  mutable std::recursive_mutex mut;
  
  std::atomic<bool> did_call;
  luabridge::LuaRef function_reference;
};

#include "LuaFunction.hh"
