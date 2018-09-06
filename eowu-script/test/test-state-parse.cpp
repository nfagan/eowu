//
//  test-state-parse.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "test-state-parse.hpp"
#include "test-util.hpp"
#include "Lua.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/test.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

void test_valid_state_with_render_functions() {
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_setup(L);
  luabridge::LuaRef invalid_setup(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto valid_file = dir + "test-redone-render-functions.lua";
  auto invalid_file = dir + "test-redone-render-functions-invalid.lua";
  
  try {
    valid_setup = util::get_global_from_script_with_trap(L, valid_file, "State");
    invalid_setup = util::get_global_from_script_with_trap(L, invalid_file, "State");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  auto res = eowu::parser::state(valid_setup);
  
  EOWU_ASSERT_TRUE(res.success, "Successfully parsed valid setup",
                   "Failed to parse valid setup with message" << res.message);
  
  auto invalid_res = eowu::parser::state(invalid_setup);
  
  EOWU_ASSERT_TRUE(!invalid_res.success, "Failed to parse invalid setup", "Succesfully parsed invalid setup");  
}

void test_valid_state() {
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_setup(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto file = dir + "test-parser-setup.lua";
  
  try {
    valid_setup = util::get_global_from_script_with_trap(L, file, "Setup");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  LuaRef states = valid_setup["States"];
  
  if (states.isNil()) {
    std::cout << "ERROR: Missing states field." << std::endl;
    return;
  }
  
  auto res = eowu::parser::states(states);
  
  try {
    if (!res.success) {
      throw std::runtime_error("Valid state schema failed to parse.");
    }
    
    auto fixation = res.result.mapping.at("Fixation");
    
    LuaRef entry_result = fixation.entry_function();
    int value = entry_result.cast<int>();
    
    if (value != 1) {
      throw std::runtime_error("Function was not handled correctly.");
    }
    
    LuaRef exit_result = fixation.exit_function();
    value = exit_result.cast<int>();
    
    if (value != 3) {
      throw std::runtime_error("Function was not handled correctly.");
    }
    
    std::cout << "OK: Fixation state parsed successfully" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: " << e.what() << std::endl;
  }
  
  
}

void test_state_parse_run_all() {
  std::cout << "--------" << "state-parser" << std::endl;
  
  test_valid_state_with_render_functions();
  test_valid_state();
}
