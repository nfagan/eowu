//
//  TargetSetWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#include "TargetSetWrapper.hpp"
#include "LuaContext.hpp"
#include "Constants.hpp"
#include <eowu-gl/eowu-gl.hpp>

#define EOWU_TS_WRAPPER_CALLBACK_SETTER(name, member, kind) \
  void eowu::TargetSetWrapper::name(luabridge::LuaRef func) { \
    if (!func.isFunction()) { \
      throw eowu::LuaError("Argument to 'kind' must be a function."); \
    } \
    member.Set(func); \
  }

eowu::TargetSetWrapper::TargetSetWrapper(std::shared_ptr<eowu::LuaContext> lua_context_,
                                         const std::vector<eowu::XYTarget*> &targets) :
lua_context(lua_context_), target_set(targets),
on_entry(lua_context_->GetState()), on_exit(lua_context_->GetState()), on_select(lua_context_->GetState()) {
  configure_callbacks();
}

void eowu::TargetSetWrapper::Begin() {
  target_set.Begin();
}

void eowu::TargetSetWrapper::Reset() {
  target_set.Reset();
}

void eowu::TargetSetWrapper::configure_callbacks() {
  target_set.SetOnEntry([&](auto *target) -> void {
    lua_context->Call(on_entry, target->GetAlias());
  });
  
  target_set.SetOnExit([&](auto *target) -> void {
    lua_context->Call(on_exit, target->GetAlias());
  });
  
  target_set.SetOnSelect([&](auto *target) -> void {
    lua_context->Call(on_select, target->GetAlias());
  });
}

EOWU_TS_WRAPPER_CALLBACK_SETTER(SetOnEntry, on_entry, "Entry");
EOWU_TS_WRAPPER_CALLBACK_SETTER(SetOnExit, on_exit, "Exit");
EOWU_TS_WRAPPER_CALLBACK_SETTER(SetOnSelect, on_select, "Select");

void eowu::TargetSetWrapper::SetDuration(int duration) {
  target_set.SetDuration(std::chrono::milliseconds(duration));
}

void eowu::TargetSetWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TargetSetWrapper>("_TargetSet")
  .addFunction("Entry", &eowu::TargetSetWrapper::SetOnEntry)
  .addFunction("Exit", &eowu::TargetSetWrapper::SetOnExit)
  .addFunction("Select", &eowu::TargetSetWrapper::SetOnSelect)
  .addFunction("Duration", &eowu::TargetSetWrapper::SetDuration)
  .addFunction("Begin", &eowu::TargetSetWrapper::Begin)
  .addFunction("Reset", &eowu::TargetSetWrapper::Reset)
  .endClass()
  .endNamespace();
}
