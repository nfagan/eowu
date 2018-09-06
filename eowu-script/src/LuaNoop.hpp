//
//  LuaNoop.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#pragma once

#include <unordered_map>
#include <memory>

struct lua_State;

namespace eowu {
  class LuaFunction;
  
  struct LuaNoop {
  public:
    static const eowu::LuaFunction& get_no_op(lua_State *L);
  private:
    static std::unordered_map<lua_State*, std::unique_ptr<eowu::LuaFunction>> no_ops;
    static const eowu::LuaFunction& make_no_op(lua_State *L);
  };
}
