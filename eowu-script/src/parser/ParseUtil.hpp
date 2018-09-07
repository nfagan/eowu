//
//  ParseUtil.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

struct lua_State;

namespace luabridge {
  class LuaRef;
}

namespace eowu {
  namespace parser {
    using MapTableType = std::unordered_map<std::string, luabridge::LuaRef>;
    
    eowu::parser::MapTableType get_string_map_from_table(const luabridge::LuaRef &ref);
    std::vector<double> get_numeric_vector_from_table(const luabridge::LuaRef &ref);
    std::vector<double> get_numeric_vector_from_state(lua_State *L, int index);
    std::vector<std::string> get_string_vector_from_table(const luabridge::LuaRef &ref);
    std::vector<std::string> get_string_vector_from_state(lua_State *L, int index);
    
    std::string get_type_error_message(const std::string &key, const std::string &type);
    std::string get_array_size_error_message(const std::string &key, int expected_size, int given_size);
    
    template<typename T>
    T get_numeric_value_or(const eowu::parser::MapTableType &table, const std::string &key, T deflt);
    
    template<typename T>
    T get_numeric_value_or_error(const eowu::parser::MapTableType &table, const std::string &key);
    
    template<typename T>
    std::vector<T> get_numeric_vector_or_type_error(const eowu::parser::MapTableType &table,
                                                    const std::string &key,
                                                    const std::vector<T> &dflt,
                                                    int size_spec = -1);
    
    luabridge::LuaRef get_function_or_error(const eowu::parser::MapTableType &table,
                                            const std::string &key);
    
    std::string get_string_or(const eowu::parser::MapTableType &table, const std::string &key, const std::string &dflt);
    
    std::string get_string_or_type_error(const eowu::parser::MapTableType &table, const std::string &key, const std::string &dflt);
    
    std::string get_string_or_error(const eowu::parser::MapTableType &table, const std::string &key);
  }
}
