//
//  VariablesWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include "parser/ParseUtil.hpp"

struct lua_State;

namespace eowu {
  class VariablesWrapper;
  class VariableWrapper;
}

class eowu::VariablesWrapper {
public:
  VariablesWrapper(eowu::parser::CommitableMapType *variables);
  
  eowu::VariableWrapper GetVariable(const std::string &name) const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  eowu::parser::CommitableMapType *variables;
};
