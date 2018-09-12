//
//  conversion.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include <eowu-data.hpp>
#include "Lua.hpp"

namespace eowu {
  namespace data {
    eowu::data::Struct from_lua(const std::string &name, const luabridge::LuaRef &value);
    void to_lua(lua_State *L, const eowu::data::Struct &s);
    
    namespace priv {
      void from_lua(const std::string &name, const luabridge::LuaRef &from, eowu::data::Struct *to);
      
      std::string get_invalid_type_error_message(const luabridge::LuaRef &from);
    }
  }
}
