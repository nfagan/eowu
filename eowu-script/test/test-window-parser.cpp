//
//  test-window-parser.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "test-window-parser.hpp"
#include "test-util.hpp"
#include "Lua.hpp"
#include <eowu-script/eowu-script.hpp>
#include <iostream>

void test_window() {
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_window(L);
  luabridge::LuaRef invalid_window(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto file = dir + "test-parser-windows.lua";
  
  try {
    valid_window = util::get_global_from_script_with_trap(L, file, "ValidWindow");
    invalid_window = util::get_global_from_script_with_trap(L, file, "InvalidWindow");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  auto v_parse_result = eowu::parser::windows(valid_window);
  auto inv_parse_result = eowu::parser::windows(invalid_window);
  
  if (!v_parse_result.success) {
    std::cout << "ERROR: failed to parse valid schema with message: " << v_parse_result.message << std::endl;
    return;
  }
  
  const auto &wins = v_parse_result.result.windows;
  
  if (wins.count("main") == 0) {
    std::cout << "ERROR: No main window specifier found" << std::endl;
    return;
  }
  
  const auto &main_win = wins.at("main");
  
  if (main_win.width != 400 || main_win.height != 400) {
    std::cout << "ERROR: Failed to parse window width and height" << std::endl;
    return;
  }
  
  std::cout << "OK: Valid window parsing was successful" << std::endl;
  
  if (inv_parse_result.success) {
    std::cout << "ERROR: invalid window succeeded to parse" << std::endl;
  } else {
    std::cout << "OK: Invalid window failed to parse with message: " << inv_parse_result.message << std::endl;
  }
}
