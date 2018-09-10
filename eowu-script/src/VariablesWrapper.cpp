//
//  VariablesWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "VariablesWrapper.hpp"
#include "VariableWrapper.hpp"
#include "Constants.hpp"
#include "Lua.hpp"

eowu::VariablesWrapper::VariablesWrapper(eowu::parser::CommitableMapType *variables_,
                                         eowu::parser::CommitableMapType *defaults_) :
variables(variables_), defaults(defaults_) {
  //
}

eowu::VariableWrapper eowu::VariablesWrapper::GetVariable(const std::string &name) const {
  auto var = &variables->at(name);
  auto dflt = &defaults->at(name);
  
  eowu::VariableWrapper wrapper(var, dflt);
  
  return wrapper;
}

void eowu::VariablesWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::VariablesWrapper>("_Variables")
  .addFunction("Field", &eowu::VariablesWrapper::GetVariable)
  .endClass()
  .endNamespace();
}
