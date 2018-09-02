//
//  test-util.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-util.hpp"
#include <eowu-script/eowu-script.hpp>
#include "Lua.hpp"

std::string util::get_lua_test_script_directory() {
  return "/Users/Nick/repositories/cpp/eowu/eowu-script/test/script/";
}

namespace detail {

  eowu::VectorWrapper* get_instance() {
    static eowu::VectorWrapper vec;
  
    return &vec;
  }
}

void util::run_lua_test_script(lua_State *L, const std::string &file) {
  using namespace luabridge;
  
  int res = luaL_dofile(L, file.c_str());
  
  if (res != 0) {
    std::cout << "File " << file << " failed to parse with message: " << lua_tostring(L, -1) << std::endl;
    return;
  }
  
  auto func = getGlobal(L, "TestCase");
  
  if (func.isNil() || !func.isFunction()) {
    std::cout << "ERROR: File " << file << " is missing TestCase." << std::endl;
    return;
  }
  
  try {
    func();
  } catch (const LuaException &e) {
    std::cout << "ERROR: " << e.what() << std::endl;
  }
}

void util::insert_vector(lua_State *L) {
  using namespace eowu;
  using namespace luabridge;
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .addFunction("GetInstance", detail::get_instance)
  .endNamespace();
  
}
