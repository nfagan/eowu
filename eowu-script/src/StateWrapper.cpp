//
//  StateWrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateWrapper.hpp"
#include "Lua.hpp"
#include <eowu-state/eowu-state.hpp>
#include <chrono>

eowu::StateWrapper::StateWrapper(eowu::State *state) {
  this->state = state;
}

void eowu::StateWrapper::SetDuration(int duration) {
  state->SetDuration(std::chrono::milliseconds(duration));
}

void eowu::StateWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace("eowu")
  .beginClass<eowu::StateWrapper>("_State")
  .addFunction("Duration", &eowu::StateWrapper::SetDuration)
  .endClass()
  .endNamespace();
}
