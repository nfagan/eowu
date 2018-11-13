//
//  ParseUtil.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "ParseUtil.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "../data/conversion.hpp"
#include <eowu-common/string.hpp>
#include <cstddef>
#include <set>

#define EOWU_get_numeric_value_or(type) \
  template type eowu::parser::get_numeric_value_or(const eowu::parser::MapTableType &table, const std::string &key, type deflt)

#define EOWU_get_numeric_value_or_error_from_state(type) \
  template type eowu::parser::get_numeric_value_or_error_from_state(lua_State *L, int index)

#define EOWU_get_numeric_value_or_error(type) \
  template type eowu::parser::get_numeric_value_or_error(const eowu::parser::MapTableType &table, const std::string &key)

#define EOWU_get_numeric_vector_or_type_error(type) \
  template std::vector<type> eowu::parser::get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table, const std::string &key, const std::vector<type> &dflt, int size_spec)

std::string eowu::parser::get_type_error_message(const std::string &key, const std::string &type) {
  return "Key '" + key + "' must refer to a value of type '" + type + "'.";
}

std::string eowu::parser::get_array_size_error_message(const std::string &key, int expected_size, int given_size) {
  return "Key '" + key + "' refers to an array of " + std::to_string(given_size) + " elements; expected " + std::to_string(expected_size) + ".";
}

std::string eowu::parser::get_extraneous_key_error_message(const std::string &key,
                                                           const std::unordered_set<std::string> &allowed) {
  
  std::set<std::string> sorted_quoted_allowed;
  
  for (const auto &one : allowed) {
    std::string quoted = "'" + one + "'";
    sorted_quoted_allowed.emplace(quoted);
  }
  
  std::string joined_allowed = eowu::util::join(sorted_quoted_allowed, ", ");
  
  return "Unrecognized key: '" + key + "'; valid keys are: " + joined_allowed + ".";
}

eowu::parser::MapTableType eowu::parser::get_string_map_from_table(const luabridge::LuaRef &table) {
  //  https://stackoverflow.com/questions/6137684/iterate-through-lua-table
  using namespace luabridge;
  eowu::parser::MapTableType result;
  
  if (!table.isTable()) {
    return result;
  }
  
  auto L = table.state();
  push(L, table);
  
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);
    const char *key = lua_tostring(L, -1);
    
    result.emplace(std::string(key), LuaRef::fromStack(L, -2));
    
    lua_pop(L, 2);
  }
  
  lua_pop(L, 1);
  
  return result;
}

//
//  array handling
//

bool eowu::parser::is_homogeneous_array(const luabridge::LuaRef &table) {
  if (!table.isTable()) {
    return false;
  }
  
  auto L = table.state();
  push(L, table);
  
  bool result = true;
  bool first_type = true;
  int last_value_type;
  
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);
    
    int key_type = lua_type(L, -1);
    int value_type = lua_type(L, -2);
    
    bool wrong_key_type = key_type != LUA_TNUMBER;
    bool nonmatching_value_types = !first_type && (value_type != last_value_type);
    
    first_type = false;
    
    if (result) {
      result = !wrong_key_type && !nonmatching_value_types;
    }
    
    last_value_type = value_type;
    
    lua_pop(L, 2);
  }
  
  lua_pop(L, 1);
  
  return result;
}

bool eowu::parser::is_string_array(const luabridge::LuaRef &table, bool check_if_array) {
  return eowu::parser::is_typed_array(table, LUA_TSTRING, check_if_array);
}

bool eowu::parser::is_numeric_array(const luabridge::LuaRef &table, bool check_if_array) {
  return eowu::parser::is_typed_array(table, LUA_TNUMBER, check_if_array);
}

bool eowu::parser::is_typed_array(const luabridge::LuaRef &table, int type, bool check_if_array) {
  if (check_if_array && !is_homogeneous_array(table)) {
    return false;
  }
  
  auto length = table.length();
  
  if (length == 0) {
    return true;
  }
  
  lua_State *L = table.state();
  push(L, table);
  lua_rawgeti(L, -1, 1);
  
  int value_type = lua_type(L, -1);
  bool result = value_type == type;
  
  lua_pop(L, 2);
  
  return result;
}

#if false
eowu::parser::MapTableType eowu::parser::get_string_map_from_table(const luabridge::LuaRef &table) {
  // https://eliasdaler.wordpress.com/2016/02/16/using-lua-with-c-iterating-over-lua-tables/
  
  using namespace luabridge;
  eowu::parser::MapTableType result;
  
  if (!table.isTable()) {
    return result;
  }

  auto L = table.state();
  push(L, table);

  lua_pushnil(L);
  
  while (lua_next(L, -2) != 0) {
    
    if (lua_isstring(L, -2)) {
      result.emplace(lua_tostring(L, -2), LuaRef::fromStack(L, -1));
    }
    
    lua_pop(L, 1);
  }

  lua_pop(L, 1);
  
  return result;
}
#endif

std::vector<double> eowu::parser::get_numeric_vector_from_table(const luabridge::LuaRef &table) {
  using namespace luabridge;
  
  std::vector<double> result;
  
  if (!table.isTable()) {
    return result;
  }
  
  for (std::size_t i = 0; i < table.length(); i++) {
    auto proxy = table[i+1];
    
    if (!proxy.isNumber()) {
      throw eowu::ScriptParseError("Expected table to be a numeric-array like object.");
    }
    
    const LuaRef ref(proxy);
    
    result.push_back(ref.cast<double>());
  }
  
  return result;
}

std::vector<double> eowu::parser::get_numeric_vector_from_state(lua_State *L, int index) {
  std::vector<double> result;
  
  if (!lua_istable(L, index)) {
    throw eowu::LuaError("Expected table to be a numeric-array like object.");
  }
  
  lua_pushvalue(L, index);
  lua_pushnil(L);
  
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);
    
    if (!lua_isnumber(L, -2)) {
      throw eowu::LuaError("Expected table to be a numeric-array like object.");
    }
    
    double value = lua_tonumber(L, -2);
    
    result.push_back(value);
    
    lua_pop(L, 2);
  }
  
  lua_pop(L, 1);
  
  return result;
}

std::vector<std::string> eowu::parser::get_string_vector_from_state(lua_State *L, int index) {
  std::vector<std::string> result;
  
  if (!lua_istable(L, index)) {
    throw eowu::LuaError("Expected table to be a string-array like object.");
  }
  
  lua_pushvalue(L, index);
  lua_pushnil(L);
  
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);
    
    if (!lua_isstring(L, -2)) {
      throw eowu::LuaError("Expected table to be a string-array like object.");
    }
    
    const char *value = lua_tostring(L, -2);
    std::string str(value);
    
    result.push_back(str);
    
    lua_pop(L, 2);
  }
  
  lua_pop(L, 1);
  
  return result;
}

std::vector<std::string> eowu::parser::get_string_vector_from_table(const luabridge::LuaRef &table) {
  using namespace luabridge;
  
  std::vector<std::string> result;
  
  if (!table.isTable()) {
    return result;
  }
  
  auto L = table.state();
  push(L, table);
  
  lua_pushnil(L);
  
  while (lua_next(L, -2)) {
    lua_pushvalue(L, -2);
    
    if (!lua_isnumber(L, -1) || !lua_isstring(L, -2)) {
      throw eowu::ScriptParseError("Expected table to be a string-array like object.");
    }
    
    const char *value = lua_tostring(L, -2);
    std::string str(value);
    
    result.push_back(str);
    
    lua_pop(L, 2);
  }
  
  lua_pop(L, 1);
  
  return result;
}

std::string eowu::parser::get_string_or_error_from_state(lua_State *L, int index) {
  if (!lua_isstring(L, index)) {
    std::string type_name = lua_typename(L, lua_type(L, index));
    throw eowu::LuaError("Expected input to be string; was: '" + type_name + "'.");
  }
  
  std::string res = lua_tostring(L, index);
  
  return res;
}

std::vector<std::string> eowu::parser::require_string_vector_from_state(lua_State *L, int index) {
  std::vector<std::string> res;
  
  if (lua_isstring(L, index)) {
    res.push_back(eowu::parser::get_string_or_error_from_state(L, index));
  } else {
    res = eowu::parser::get_string_vector_from_state(L, index);
  }
  
  return res;
}

#if false
std::vector<std::string> eowu::parser::get_string_vector_from_table(const luabridge::LuaRef &table) {
  using namespace luabridge;
  
  std::vector<std::string> result;
  
  if (!table.isTable()) {
    return result;
  }
  
  for (std::size_t i = 0; i < table.length(); i++) {
    auto proxy = table[i+1];
    
    if (!proxy.isString()) {
      throw eowu::ScriptParseError("Expected table to be a string-array like object.");
    }
    
    const LuaRef ref(proxy);
    
    result.push_back(ref.tostring());
  }
  
  return result;
}
#endif

//
//  get variables
//

eowu::parser::VariableMapType eowu::parser::get_variables(const luabridge::LuaRef &ref) {
  eowu::parser::VariableMapType result;
  
  auto kv = eowu::parser::get_string_map_from_table(ref);
  
  for (const auto &it : kv) {
    const std::string &name = it.first;
    const luabridge::LuaRef &ref = it.second;
    
    auto parsed_struct = eowu::data::from_lua(name, ref);
    
    eowu::data::Commitable commitable;
    commitable.Set(parsed_struct);
    
    result.emplace(name, commitable);
  }
  
  return result;
}

//
//  get_numeric_value_or
//

template<typename T>
T eowu::parser::get_numeric_value_or(const eowu::parser::MapTableType &table,
                                     const std::string &key,
                                     T deflt) {
  
  if (table.count(key) == 0) {
    return deflt;
  }
  
  const auto &ref = table.at(key);
  
  bool is_bool = ref.isBool();
  
  if (!ref.isNumber() && !is_bool) {
    return deflt;
  } else if (is_bool) {
    bool res = ref.cast<bool>();
    return static_cast<T>(res);
  }
  
  return ref.cast<T>();
}

EOWU_get_numeric_value_or(int);
EOWU_get_numeric_value_or(unsigned int);
EOWU_get_numeric_value_or(bool);
EOWU_get_numeric_value_or(double);

//
//  get_numeric_value_or_error_from_state
//

template<typename T>
T eowu::parser::get_numeric_value_or_error_from_state(lua_State *L, int index) {
  
  if (!lua_isnumber(L, index)) {
    std::string type_name = lua_typename(L, lua_type(L, index));
    throw eowu::LuaError("Expected input to be numeric; was: '" + type_name + "'.");
  }
  
  double num = lua_tonumber(L, index);
  
  return static_cast<T>(num);
}

EOWU_get_numeric_value_or_error_from_state(double);
EOWU_get_numeric_value_or_error_from_state(int);

bool eowu::parser::get_bool_or_error_from_state(lua_State *L, int index) {
  if (!lua_isboolean(L, index)) {
    std::string type_name = lua_typename(L, lua_type(L, index));
    throw eowu::LuaError("Expected input to be boolean; was: '" + type_name + "'.");
  }
  
  return lua_toboolean(L, index);
}

//
//  get_numeric_value_or_error
//

template<typename T>
T eowu::parser::get_numeric_value_or_error(const eowu::parser::MapTableType &table,
                                           const std::string &key) {
  
  if (table.count(key) == 0) {
    throw eowu::ScriptParseError("Missing key: " + key);
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isNumber()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "number"));
  }
  
  return ref.cast<T>();
}

EOWU_get_numeric_value_or_error(int);
EOWU_get_numeric_value_or_error(unsigned int);
EOWU_get_numeric_value_or_error(double);

//
//  get_numeric_vector_or_type_error
//

template<typename T>
std::vector<T> eowu::parser::get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table,
                                                              const std::string &key,
                                                              const std::vector<T> &dflt,
                                                              int size_spec) {
  
  if (table.count(key) == 0) {
    return dflt;
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isTable()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "table"));
  }
  
  std::vector<T> res;
  
  auto dbl_vec = eowu::parser::get_numeric_vector_from_table(ref);
  
  std::size_t n_els = dbl_vec.size();
  
  if (size_spec >= 0 && n_els != size_spec) {
    throw eowu::ScriptParseError(eowu::parser::get_array_size_error_message(key, size_spec, n_els));
  }
  
  for (std::size_t i = 0; i < n_els; i++) {
    res.push_back(static_cast<T>(dbl_vec[i]));
  }
  
  return res;
}

template<>
std::vector<double> eowu::parser::get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table,
                                                                   const std::string &key,
                                                                   const std::vector<double> &dflt,
                                                                   int size_spec) {
  
  if (table.count(key) == 0) {
    return dflt;
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isTable()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "table"));
  }
  
  auto vec = eowu::parser::get_numeric_vector_from_table(ref);
  
  if (size_spec >= 0 && vec.size() != size_spec) {
    throw eowu::ScriptParseError(eowu::parser::get_array_size_error_message(key, size_spec, vec.size()));
  }
  
  return vec;
}

EOWU_get_numeric_vector_or_type_error(int);
EOWU_get_numeric_vector_or_type_error(unsigned int);

//
//  string
//

std::string eowu::parser::get_string_or(const eowu::parser::MapTableType &table,
                                        const std::string &key,
                                        const std::string &dflt) {
  
  if (table.count(key) == 0) {
    return dflt;
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isString()) {
    return dflt;
  }
  
  return ref.tostring();
}

std::string eowu::parser::get_string_or_type_error(const eowu::parser::MapTableType &table,
                                                   const std::string &key, const std::string &dflt) {
  
  if (table.count(key) == 0) {
    return dflt;
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isString()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "string"));
  }
  
  return ref.tostring();
}

std::string eowu::parser::get_string_or_error(const eowu::parser::MapTableType &table,
                                              const std::string &key) {
  if (table.count(key) == 0) {
    throw eowu::ScriptParseError("Missing key: " + key);
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isString()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "string"));
  }
  
  return ref.tostring();
}

//
//  function
//

const luabridge::LuaRef& eowu::parser::get_function_or_error(const eowu::parser::MapTableType &table,
                                                             const std::string &key) {
  if (table.count(key) == 0) {
    throw eowu::ScriptParseError("Missing key: " + key);
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isFunction()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "function"));
  }
  
  return ref;
}

const luabridge::LuaRef& eowu::parser::get_function_or_type_error(const eowu::parser::MapTableType &table,
                                                                  const std::string &key,
                                                                  const luabridge::LuaRef &dflt) {
  if (table.count(key) == 0) {
    return dflt;
  }
  
  const auto &ref = table.at(key);
  
  if (!ref.isFunction()) {
    throw eowu::ScriptParseError(eowu::parser::get_type_error_message(key, "function"));
  }
  
  return ref;
}

eowu::parser::ExtraneousKeyResult eowu::parser::has_extraneous_key(const std::unordered_set<std::string> &allowed,
                                                                   const std::unordered_set<std::string> &test) {
  eowu::parser::ExtraneousKeyResult result;
  
  for (const auto &val : test) {
    if (allowed.count(val) == 0) {
      result.has_extraneous = true;
      result.name = val;
      
      return result;
    }
  }
  
  return result;
}



