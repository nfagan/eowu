//
//  LuaNoop.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#include "LuaNoop.hpp"
#include "LuaFunction.hpp"
#include "Constants.hpp"
#include <Lua.hpp>

//  globals
std::unordered_map<lua_State*, std::unique_ptr<eowu::LuaFunction>> eowu::LuaNoop::no_ops;

const eowu::LuaFunction& eowu::LuaNoop::get_no_op(lua_State *L) {
  const auto &it = eowu::LuaNoop::no_ops.find(L);
  
  if (it == eowu::LuaNoop::no_ops.end()) {
    return eowu::LuaNoop::make_no_op(L);
  } else {
    return *it->second.get();
  }
}

const eowu::LuaFunction& eowu::LuaNoop::make_no_op(lua_State *L) {
  std::string full_noop = std::string("function ") + eowu::constants::eowu_noop_name + "() end";
  
  const char* const buff = full_noop.c_str();
  int error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
  
  assert(error == 0);
  
  luabridge::LuaRef no_op_global = luabridge::getGlobal(L, eowu::constants::eowu_noop_name);
  
  assert(no_op_global.isFunction());
  
  auto func = std::make_unique<eowu::LuaFunction>(eowu::LuaReferenceContainer(no_op_global));
  
  eowu::LuaNoop::no_ops.emplace(L, std::move(func));
  
  return *eowu::LuaNoop::no_ops.at(L).get();
}
