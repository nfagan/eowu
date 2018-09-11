//
//  VariableWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "VariableWrapper.hpp"
#include "Constants.hpp"
#include "Error.hpp"
#include "data/conversion.hpp"

eowu::VariableWrapper::VariableWrapper(eowu::VariableWrapper::VariableDataType data_, eowu::VariableWrapper::VariableDataType default_value_) :
data(data_), default_value(default_value_) {
  //
}

int eowu::VariableWrapper::Get(lua_State *L) {  
  const auto functor = [&](const auto& val) {
    eowu::data::to_lua(L, val);
  };
  
  data->Use(functor);
  
  return 1;
}

int eowu::VariableWrapper::Set(lua_State *L) {
  int inputs = lua_gettop(L);
  
  std::string name;
  
  const auto name_getter = [&](auto &use) -> void {
    name = use.name;
  };
  
  data->Use(name_getter);
  
  if (inputs == 1) {
    throw eowu::LuaError("Attempted to assign a nil value to variable: '" + name + "'.");
  }
  
  luabridge::LuaRef table = luabridge::LuaRef::fromStack(L, -1);
  data->Set(eowu::data::from_lua(name, table));
  
  return 0;
}

void eowu::VariableWrapper::Reset() {
  const auto user = [&](auto &use) -> void {
    data->Uncommit();
    data->Set(use);
  };
  
  default_value->Use(user);
}

void eowu::VariableWrapper::Commit() {
  data->Commit();
}

void eowu::VariableWrapper::Uncommit() {
  data->Uncommit();
}

void eowu::VariableWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::VariableWrapper>("_Variable")
  .addFunction("Commit", &eowu::VariableWrapper::Commit)
  .addFunction("Uncommit", &eowu::VariableWrapper::Uncommit)
  .addCFunction("Set", &eowu::VariableWrapper::Set)
  .addCFunction("Get", &eowu::VariableWrapper::Get)
  .addFunction("Reset", &eowu::VariableWrapper::Reset)
  .endClass()
  .endNamespace();
}
