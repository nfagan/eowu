//
//  test-parse-util.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "test-parse-util.hpp"
#include "test-util.hpp"
#include "Lua.hpp"
#include <eowu-script/eowu-script.hpp>
#include <stdexcept>
#include <vector>
#include <string>

void test_string_vector_or_error() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_array1(L);
  luabridge::LuaRef valid_array2(L);
  luabridge::LuaRef invalid_array1(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto file = dir + "test-parser-util-string-vector.lua";
  
  try {
    valid_array1 = util::get_global_from_script_with_trap(L, file, "ValidVector1");
    valid_array2 = util::get_global_from_script_with_trap(L, file, "ValidVector2");
    invalid_array1 = util::get_global_from_script_with_trap(L, file, "InvalidVector1");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  //
  //  valid, filled
  //
  
  std::vector<std::string> res;
  
  try {
    res = eowu::parser::get_string_vector_from_table(valid_array1);
    
    if (res.size() != 3) {
      throw std::runtime_error("Expected size to be 3");
    }
    
    if (res[0] != "a" || res[1] != "b" || res[2] != "cd") {
      throw std::runtime_error("Contents were not equal.");
    }
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: parsing valid string filled vector failed with message: " << e.what() << std::endl;
  }
  
  std::cout << "OK: valid string filled vector parsed successfully" << std::endl;
  
  //
  //  valid, empty
  //
  
  try {
    res = eowu::parser::get_string_vector_from_table(valid_array2);
    
    if (res.size() != 0) {
      throw std::runtime_error("Expected size to be 0");
    }
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: parsing empty string vector failed with message: " << e.what() << std::endl;
  }
  
  std::cout << "OK: valid empty string vector parsed successfully" << std::endl;
  
  //
  //  invalid
  //
  
  try {
    res = eowu::parser::get_string_vector_from_table(invalid_array1);
    std::cout << "ERROR: Successfully parsed invalid string vector." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "OK: Attempting to parse an invalid vector failed with message: " << e.what() << std::endl;
  }
}

void test_numeric_vector_or_error() {
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_array1(L);
  luabridge::LuaRef valid_array2(L);
  luabridge::LuaRef invalid_array1(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto file = dir + "test-parser-util-numeric-vector.lua";
  
  try {
    valid_array1 = util::get_global_from_script_with_trap(L, file, "ValidVector1");
    valid_array2 = util::get_global_from_script_with_trap(L, file, "ValidVector2");
    invalid_array1 = util::get_global_from_script_with_trap(L, file, "InvalidVector1");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  //
  //  valid, filled
  //
  
  std::vector<double> res;
  
  try {
    res = eowu::parser::get_numeric_vector_from_table(valid_array1);
    
    if (res.size() != 3) {
      throw std::runtime_error("Expected size to be 3");
    }
    
    if (res[0] != 1.0 || res[1] != 2.0 || res[2] != 3.0) {
      throw std::runtime_error("Contents were not equal.");
    }
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: parsing valid filled vector failed with message: " << e.what() << std::endl;
  }
  
  std::cout << "OK: valid filled vector parsed successfully" << std::endl;
  
  //
  //  valid, empty
  //
  
  try {
    res = eowu::parser::get_numeric_vector_from_table(valid_array2);
    
    if (res.size() != 0) {
      throw std::runtime_error("Expected size to be 0");
    }
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: parsing empty vector failed with message: " << e.what() << std::endl;
  }
  
  std::cout << "OK: valid empty vector parsed successfully" << std::endl;
  
  //
  //  invalid
  //
  
  try {
    res = eowu::parser::get_numeric_vector_from_table(invalid_array1);
    std::cout << "ERROR: Successfully parsed invalid vector." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "OK: Attempting to parse an invalid vector failed with message: " << e.what() << std::endl;
  }
  
}

void test_parse_util_run_all() {
  std::cout << "--------" << "parser-util" << std::endl;
  
  test_string_vector_or_error();
  test_numeric_vector_or_error();
}
