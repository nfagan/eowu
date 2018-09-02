//
//  test-util.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

struct lua_State;

namespace luabridge {
  class LuaRef;
}

#include <iostream>

namespace util {
  
  void insert_vector(lua_State *L);
  void run_lua_test_script(lua_State *L, const std::string &file);
  std::string get_lua_test_script_directory();
  luabridge::LuaRef get_global_from_script_with_trap(lua_State *L, const std::string &file, const std::string &name);
  
}
