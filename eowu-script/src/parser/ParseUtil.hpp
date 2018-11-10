//
//  ParseUtil.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include "ParseResult.hpp"
#include <eowu-data.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

struct lua_State;

namespace luabridge {
  class LuaRef;
}

namespace eowu {
  namespace parser {
    using MapTableType = std::unordered_map<std::string, luabridge::LuaRef>;
    using VariableMapType = std::unordered_map<std::string, eowu::data::Commitable>;
    using CommitableMapType = std::unordered_map<std::string, eowu::data::Commitable>;
    
    eowu::parser::MapTableType get_string_map_from_table(const luabridge::LuaRef &ref);
    std::vector<double> get_numeric_vector_from_table(const luabridge::LuaRef &ref);
    std::vector<double> get_numeric_vector_from_state(lua_State *L, int index);
    std::vector<std::string> get_string_vector_from_table(const luabridge::LuaRef &ref);
    std::vector<std::string> get_string_vector_from_state(lua_State *L, int index);
    
    eowu::parser::VariableMapType get_variables(const luabridge::LuaRef &ref);
    
    template<typename T>
    eowu::parser::ParseResult<T> parse_string_map_from_table(const luabridge::LuaRef &table,
                                                             const std::string &context);
    
    bool is_homogeneous_array(const luabridge::LuaRef &table);
    bool is_typed_array(const luabridge::LuaRef &table, int type, bool check_if_array = true);
    bool is_string_array(const luabridge::LuaRef &table, bool check_if_array = true);
    bool is_numeric_array(const luabridge::LuaRef &table, bool check_if_array = true);
    
    std::string get_type_error_message(const std::string &key, const std::string &type);
    std::string get_array_size_error_message(const std::string &key, int expected_size, int given_size);
    std::string get_extraneous_key_error_message(const std::string &key, const std::unordered_set<std::string> &allowed);
    
    template<typename T>
    T get_numeric_value_or(const eowu::parser::MapTableType &table, const std::string &key, T deflt);
    
    template<typename T>
    T get_numeric_value_or_error(const eowu::parser::MapTableType &table, const std::string &key);
    
    template<typename T>
    std::vector<T> get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table,
                                                    const std::string &key,
                                                    const std::vector<T> &dflt,
                                                    int size_spec = -1);
    
    const luabridge::LuaRef& get_function_or_error(const eowu::parser::MapTableType &table,
                                                   const std::string &key);
    const luabridge::LuaRef& get_function_or_type_error(const eowu::parser::MapTableType &table,
                                                        const std::string &key,
                                                        const luabridge::LuaRef &dflt);
    
    std::string get_string_or(const eowu::parser::MapTableType &table, const std::string &key, const std::string &dflt);
    
    std::string get_string_or_type_error(const eowu::parser::MapTableType &table, const std::string &key, const std::string &dflt);
    
    std::string get_string_or_error(const eowu::parser::MapTableType &table, const std::string &key);
    
    struct ExtraneousKeyResult {
      std::string name;
      bool has_extraneous;
      
      ExtraneousKeyResult() : has_extraneous(false) {}
    };
    
    ExtraneousKeyResult has_extraneous_key(const std::unordered_set<std::string> &allowed,
                                           const std::unordered_set<std::string> &test);
  }
}

#include "ParseUtil.hh"
