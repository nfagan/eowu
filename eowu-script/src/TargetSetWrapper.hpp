//
//  TargetSetWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#pragma once

#include "Lua.hpp"
#include "LuaFunction.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <memory>
#include <vector>
#include <functional>

struct lua_State;

namespace eowu {
  class TargetSetWrapper;
  class XYTarget;
  class LuaContext;
}

class eowu::TargetSetWrapper {
public:
  TargetSetWrapper(std::shared_ptr<eowu::LuaContext> lua_context,
                   std::shared_ptr<eowu::XYTargetSet> target_set);
  
  ~TargetSetWrapper() = default;
  
  void Begin();
  void Reset();
  
  void SetOnEntry(luabridge::LuaRef func);
  void SetOnExit(luabridge::LuaRef func);
  void SetOnSelect(luabridge::LuaRef func);
  void SetDuration(int duration);
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::LuaContext> lua_context;
  std::shared_ptr<eowu::XYTargetSet> target_set;
  
  eowu::LuaFunction on_entry;
  eowu::LuaFunction on_exit;
  eowu::LuaFunction on_select;
  
  const std::function<void(eowu::XYTarget*)> get_callback(luabridge::LuaRef func);
};
