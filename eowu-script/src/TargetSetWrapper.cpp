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

#define EOWU_TS_SET_CALLBACK(name, member) \
  member.Set(func); \
                      \
  target_set->name([&](auto *targ) -> void { \
    lua_context->Call(member, targ->GetAlias()); \
  });

eowu::TargetSetWrapper::TargetSetWrapper(std::shared_ptr<eowu::LuaContext> lua_context_,
                                         std::shared_ptr<eowu::XYTargetSet> target_set_) :
lua_context(lua_context_),
target_set(target_set_),
on_entry(lua_context_->GetState()),
on_exit(lua_context_->GetState()),
on_select(lua_context_->GetState()) {
  
}

void eowu::TargetSetWrapper::Begin() {
  target_set->Begin();
}

void eowu::TargetSetWrapper::Reset() {
  target_set->Reset();
}

void eowu::TargetSetWrapper::SetOnEntry(luabridge::LuaRef func) {
  EOWU_TS_SET_CALLBACK(SetOnEntry, on_entry);
}

void eowu::TargetSetWrapper::SetOnExit(luabridge::LuaRef func) {
  EOWU_TS_SET_CALLBACK(SetOnExit, on_exit);
}

void eowu::TargetSetWrapper::SetOnSelect(luabridge::LuaRef func) {
  EOWU_TS_SET_CALLBACK(SetOnSelect, on_select);
}

void eowu::TargetSetWrapper::SetDuration(int duration) {
  target_set->SetDuration(std::chrono::milliseconds(duration));
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
