//
//  LuaContext.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaContext.hpp"
#include "LuaFunction.hpp"

eowu::LuaContext::LuaContext() {
  lua_state = nullptr;
}

eowu::LuaContext::LuaContext(lua_State *state) {
  lua_state = state;
}

lua_State* eowu::LuaContext::GetState() const {
  return lua_state;
}

void eowu::LuaContext::SetState(lua_State *L) {
  lua_state = L;
}

void eowu::LuaContext::Call(const eowu::LuaFunction &other) const {
  std::unique_lock<std::mutex> lock(mut);
  other.Call();
}
