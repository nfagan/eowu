//
//  TimeoutWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#include "TimeoutWrapper.hpp"
#include "Constants.hpp"

void eowu::TimeoutWrapper::Update() {
  timeout.Update();
}

void eowu::TimeoutWrapper::Reset() {
  timeout.Reset();
}

void eowu::TimeoutWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TimeoutWrapper>("_Timeout")
  .endClass()
  .endNamespace();
}
