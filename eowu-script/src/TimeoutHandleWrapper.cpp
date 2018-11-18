//
//  TimeoutHandleWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/17/18.
//

#include "TimeoutHandleWrapper.hpp"
#include "TimeoutHandle.hpp"
#include "Constants.hpp"
#include "Lua.hpp"

eowu::TimeoutHandleWrapper::TimeoutHandleWrapper(std::shared_ptr<eowu::TimeoutHandle> handle) :
timeout_handle(handle) {
  //
}

void eowu::TimeoutHandleWrapper::Cancel() {
  timeout_handle->Cancel();
}

void eowu::TimeoutHandleWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TimeoutHandleWrapper>("_TimeoutHandle")
  .addFunction("Cancel", &eowu::TimeoutHandleWrapper::Cancel)
  .endClass()
  .endNamespace();
}
