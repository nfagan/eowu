//
//  LuaStateManager.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "TaskWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"
#include "StateWrapper.hpp"
#include <functional>

eowu::StateWrapperContainerType eowu::TaskWrapper::States = nullptr;

eowu::StateWrapper* eowu::TaskWrapper::GetStateWrapper(const std::string &id) {
  return States->at(id).get();
}

void eowu::TaskWrapper::CreateLuaSchema(lua_State *L) {
  using namespace luabridge;
  
  getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TaskWrapper>("Task")
  .addStaticFunction("State", &eowu::TaskWrapper::GetStateWrapper)
  .endClass()
  .endNamespace();
}
