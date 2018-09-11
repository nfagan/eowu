//
//  VariableWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include "Lua.hpp"
#include <eowu-data.hpp>

namespace eowu {
  class VariableWrapper;
}

class eowu::VariableWrapper {
  using VariableDataType = eowu::data::Commitable*;
  
public:
  VariableWrapper(VariableDataType data, VariableDataType default_value);
  
  void Commit();
  void Uncommit();
  void Reset();
  int Get(lua_State *L);
  int Set(lua_State *L);
  
  static void CreateLuaSchema(lua_State *L);
private:
  
  VariableDataType data;
  VariableDataType default_value;
};
