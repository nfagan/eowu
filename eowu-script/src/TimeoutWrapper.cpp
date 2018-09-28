//
//  TimeoutWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#include "TimeoutWrapper.hpp"
#include "LuaContext.hpp"
#include "Constants.hpp"

eowu::TimeoutWrapper::TimeoutWrapper(std::shared_ptr<eowu::LuaContext> context,
                                     const luabridge::LuaRef &func,
                                     const eowu::time::DurationType &duration) :
timeout(duration), lua_context(context), on_ellapsed(func) {
  configure_timeout();
}

void eowu::TimeoutWrapper::Update() {
  timeout.Update();
}

void eowu::TimeoutWrapper::Cancel() {
  timeout.Reset();
}

void eowu::TimeoutWrapper::configure_timeout() {
  timeout.Reset();
  timeout.SetOnEllapsed([&]() -> void {
    lua_context->Call(on_ellapsed);
    Cancel();
  });
}

void eowu::TimeoutWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TimeoutWrapper>("_Timeout")
  .addFunction("Cancel", &eowu::TimeoutWrapper::Cancel)
  .endClass()
  .endNamespace();
}
