//
//  test-util.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-util.hpp"
#include <eowu-script/eowu-script.hpp>
#include "Lua.hpp"
#include <eowu-common/path.hpp>
#include <stdexcept>

std::string util::get_lua_test_script_directory() {
  return eowu::path::get_eowu_root_directory() + "/eowu-script/test/script/";
}

namespace detail {

  eowu::VectorWrapper* get_instance() {
    static eowu::VectorWrapper vec;
  
    return &vec;
  }
}

luabridge::LuaRef util::get_global_from_script_with_trap(lua_State *L, const std::string &file, const std::string &name) {
  using namespace luabridge;
  
  int res = luaL_dofile(L, file.c_str());
  lua_pcall(L, 0, 0, 0);
  
  if (res != 0) {
    auto msg = "File " + file + " failed to parse with message: " + lua_tostring(L, -1);
    throw std::runtime_error(msg);
  }
  
  auto glob = getGlobal(L, name.c_str());
  
  if (glob.isNil()) {
    throw std::runtime_error("Global variable " + name + " does not exist.");
  }
  
  return glob;
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
