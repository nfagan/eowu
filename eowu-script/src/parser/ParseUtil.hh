//
//  ParseUtil.hh
//  eowu
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include "Lua.hpp"

template<typename T>
eowu::parser::ParseResult<T> eowu::parser::parse_string_map_from_table(const luabridge::LuaRef &table,
                                                                       const std::string &context) {
  eowu::parser::ParseResult<T> result;
  
  auto kv = eowu::parser::get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    const auto &key = it.first;
    const auto &val = it.second;
    
    if (!val.isString()) {
      result.message = "Key '" + key + "' must refer to a string value.";
      result.context = context;
      return result;
    }
    
    result.result.mapping.emplace(key, val.tostring());
  }
  
  result.success = true;
  
  return result;
}
