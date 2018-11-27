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
  static auto start_time = eowu::time::now();
  
  auto c_time = eowu::time::now();
  auto micro_secs = std::chrono::duration_cast<std::chrono::microseconds>(c_time - start_time);
  
  return micro_secs.count();
}
