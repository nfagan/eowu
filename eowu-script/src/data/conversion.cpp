//
//  conversion.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "conversion.hpp"
#include "../parser/ParseUtil.hpp"
#include "Error.hpp"
#include <cstddef>

namespace priv {
  struct insert_visitor {
    lua_State *L;
    
    insert_visitor(lua_State *L_) : L(L_) {}
    
    //  value is scalar number
    void operator()(double val) {
      lua_pushnumber(L, val);
    }
    
    //  value is scalar boolean
    void operator()(bool val) {
      lua_pushboolean(L, val);
    }
    
    //  value is scalar string
    void operator()(const std::string &val) {
      lua_pushstring(L, val.c_str());
    }
    
    //  value is one-dimensional string array: {'hi', 'hello'}
    void operator()(const std::vector<std::string> &val) {
      lua_newtable(L);
      
      std::size_t N = val.size();
      
      for (std::size_t i = 0; i < N; i++) {
        lua_pushstring(L, val[i].c_str());
        lua_rawseti(L, -2, i+1);
      }
    }
    
    //  value is one-dimensional double array: {1.0, 2.0, 3.0}
    void operator()(const std::vector<double> &val) {
      lua_newtable(L);
      
      std::size_t N = val.size();
      
      for (std::size_t i = 0; i < N; i++) {
        lua_pushnumber(L, val[i]);
        lua_rawseti(L, -2, i+1);
      }
    }
    
    //  value is map or multidimensional array: {hi = 10, 1, 2, 3, b = {another = 10}}
    void operator()(const std::vector<eowu::data::Struct> &other) {
      lua_newtable(L);
      
      for (const auto &it : other) {
        insert_visitor local_visitor{L};
        mpark::visit(local_visitor, it.value);
        
        lua_setfield(L, -2, it.name.c_str());
      }
    }
  };
}

void eowu::data::to_lua(lua_State *L, const eowu::data::Struct &s) {
  mpark::visit(::priv::insert_visitor{L}, s.value);
}

void eowu::data::to_lua(lua_State *L, const std::vector<std::string> &value) {
  ::priv::insert_visitor{L}(value);
}

eowu::data::Struct eowu::data::from_lua(const std::string &name, const luabridge::LuaRef &value) {
  eowu::data::Struct result;
  
  priv::from_lua(name, value, &result);
  
  return result;
}

void eowu::data::priv::from_lua(const std::string &name, const luabridge::LuaRef &from, eowu::data::Struct *to) {
  to->name = name;
  
  //  value is bool
  if (from.isBool()) {
    to->value = from.cast<bool>();
  
  //  value is double
  } else if (from.isNumber()) {
    to->value = from.cast<double>();
    
  //  value is string
  } else if (from.isString()) {
    to->value = from.cast<std::string>();
    
  //  value is either {['key'] = value} or {1, 2, 3} or {'hi', 'hello'}
  //  mixed types like {1, 2, a = 3} are interperted as map types where
  //  numeric keys are implicitly converted to string.
  } else if (from.isTable()) {
    
    //  value is one-dimensional array: {1, 2, 3} or {'hi', 'hello'}
    if (parser::is_homogeneous_array(from)) {
      bool check_array = false;
      
      if (parser::is_numeric_array(from, check_array)) {
        to->value = parser::get_numeric_vector_from_table(from);
        
      } else if (parser::is_string_array(from, check_array)) {
        to->value = parser::get_string_vector_from_table(from);
        
      } else {
        throw eowu::DataConversionError(priv::get_invalid_type_error_message(from));
      }
      
    //  value is key-value map or multi-dimensional array
    } else {
      auto kv = eowu::parser::get_string_map_from_table(from);
      
      std::vector<eowu::data::Struct> fields;
      
      for (const auto &it : kv) {
        const auto &inner_name = it.first;
        const luabridge::LuaRef &ref = it.second;
        
        eowu::data::Struct field;
        eowu::data::priv::from_lua(inner_name, ref, &field);
        
        fields.push_back(field);
      }
      
      to->value = fields;
    }
  } else {    
    throw eowu::DataConversionError(priv::get_invalid_type_error_message(from));
  }
}

std::string eowu::data::priv::get_invalid_type_error_message(const luabridge::LuaRef &from) {
  const char *type_name = lua_typename(from.state(), from.type());
  return "No viable conversion to variable-data from the provided Lua type: '" + std::string(type_name) + "'.";
}
