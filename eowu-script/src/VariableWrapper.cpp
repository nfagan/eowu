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

eowu::VariableWrapper::VariableWrapper(eowu::VariableWrapper::VariableDataType data_) :
data(data_) {
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

void eowu::VariableWrapper::Commit() {
  data->Commit();
}

void eowu::VariableWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::VariableWrapper>("_Variable")
  .addFunction("Commit", &eowu::VariableWrapper::Commit)
  .addCFunction("Set", &eowu::VariableWrapper::Set)
  .addCFunction("Get", &eowu::VariableWrapper::Get)
  .endClass()
  .endNamespace();
}
