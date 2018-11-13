//
//  TimeWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/13/18.
//

#include "TimeWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"
#include <eowu-common/time.hpp>

void eowu::time::wrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginNamespace(eowu::constants::eowu_time_namespace)
  .addFunction("now", &eowu::time::wrapper::now)
  .endNamespace()
  .endNamespace();
}

double eowu::time::wrapper::now() {
  auto c_time = eowu::time::now();
  auto msecs = std::chrono::time_point_cast<std::chrono::milliseconds>(c_time);
  
  return msecs.time_since_epoch().count();
}
