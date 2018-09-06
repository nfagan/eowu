//
//  test-entry-parse.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "test-entry-parse.hpp"
#include "test-util.hpp"
#include <eowu-state/eowu-state.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/test.hpp>

void test::entry::run_all() {
  std::cout << "--------" << "entry script" << std::endl;
  
  test_valid_entry();
  test_invalid_entry();
}

void test::entry::test_valid_entry() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  auto file = util::get_lua_test_script_directory() + "test-valid-entry-script.lua";
  auto res = eowu::parser::entry_script(L, file.c_str());
  
  EOWU_ASSERT_TRUE(res.success, "Valid entry file parsed successfully.",
                   "Valid entry file failed to parse with message: " + res.message);
}

void test::entry::test_invalid_entry() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  auto file = util::get_lua_test_script_directory() + "test-invalid-entry-non-table-script.lua";
  auto res = eowu::parser::entry_script(L, file.c_str());
  
  EOWU_ASSERT_TRUE(!res.success, "Invalid entry file was not valid: " + res.message,
                   "Invalid entry file parsed successfully.");
}
