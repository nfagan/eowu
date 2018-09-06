//
//  ParseUtil.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "ParseUtil.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include <cstddef>

#define EOWU_get_numeric_value_or(type) \
  template type eowu::parser::get_numeric_value_or(const eowu::parser::MapTableType &table, const std::string &key, type deflt)

#define EOWU_get_numeric_value_or_error(type) \
  template type eowu::parser::get_numeric_value_or_error(const eowu::parser::MapTableType &table, const std::string &key)

#define EOWU_get_numeric_vector_or_type_error(type) \
  template std::vector<type> eowu::parser::get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table, const std::string &key, const std::vector<type> &dflt, int size_spec)

std::string eowu::parser::get_type_error_message(const std::string &key, const std::string &type) {
  return "Key '" + key + "' does not refer to the expected type '" + type + "'.";
}

std::string eowu::parser::get_array_size_error_message(const std::string &key, int expected_size, int given_size) {
  return "Key '" + key + "' refers to an array of " + std::to_string(given_size) + " elements; expected " + std::to_string(expected_size) + ".";
}

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
  
  if (!ref.isNumber()) {
    return deflt;
  }
  
  return ref.cast<T>();
}

EOWU_get_numeric_value_or(int);
EOWU_get_numeric_value_or(unsigned int);
EOWU_get_numeric_value_or(double);

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

luabridge::LuaRef eowu::parser::get_function_or_error(const eowu::parser::MapTableType &table,
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



