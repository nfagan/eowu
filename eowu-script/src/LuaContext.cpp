//
//  LuaContext.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaContext.hpp"

eowu::LuaContext::LuaContext() : lua_state(nullptr) {
  //
}

eowu::LuaContext::LuaContext(lua_State *state) {
  SetState(state);
}

lua_State* eowu::LuaContext::GetState() const {
  return lua_state;
}

void eowu::LuaContext::SetState(lua_State *L) {
  lua_state = L;
  
  lua_gc(lua_state, LUA_GCSETPAUSE, 50);
  lua_gc(lua_state, LUA_GCSETSTEPMUL, 400);
}
