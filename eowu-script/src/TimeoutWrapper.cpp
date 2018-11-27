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
                                     eowu::Timeout::Type timeout_type,
                                     const eowu::time::DurationType &duration) :
timeout(timeout_type, duration), lua_context(context), callback(func), is_canceled(false) {
  configure_timeout();
}

eowu::TimeoutWrapper::~TimeoutWrapper() {
  Cancel();
}

void eowu::TimeoutWrapper::Update() {
  timeout.Update();
}

void eowu::TimeoutWrapper::Cancel() {
  timeout.Cancel();
  is_canceled = true;
}

bool eowu::TimeoutWrapper::IsCancelled() const {
  return is_canceled;
}

void eowu::TimeoutWrapper::configure_timeout() {
  timeout.Reset();
  timeout.SetCallback([&]() -> void {
    lua_context->Call(callback);
    
    if (timeout.GetType() == eowu::Timeout::TIMEOUT) {
      is_canceled = true;
    }
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
