//
//  test-path.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "test-path.hpp"
#include <eowu-common/path.hpp>
#include <eowu-common/test.hpp>
#include <eowu-common/platform.hpp>
#include <iostream>

void eowu::test::test_path() {
  std::string platform = eowu::platform::name;
  
  std::string file_path_1 = "/Users/Nick/test";
  
  auto outer_dir = eowu::path::get_outer_directory(file_path_1);
  
  EOWU_ASSERT_TRUE(outer_dir == "/Users/Nick", "Outer directory parsing succeeded on: " + platform,
                   "Outer directory parsing failed on: " + platform + "; value was: " + outer_dir);
  
  //  test trailing slash
  std::string file_path_2 = "/Users/Nick/test/";
  
  outer_dir = eowu::path::get_outer_directory(file_path_2);
  
  EOWU_ASSERT_TRUE(outer_dir == "/Users/Nick", "Outer directory parsing with trailing slash succeeded on: " + platform,
                   "Outer directory parsing with trailing slash failed on: " + platform + "; value was: " + outer_dir);
  
  //  test no slashes
  std::string file_path_3 = "Users";
  
  outer_dir = eowu::path::get_outer_directory(file_path_3);
  
  EOWU_ASSERT_TRUE(outer_dir == file_path_3, "Outer directory parsing with no slashes succeeded on: " + platform,
                   "Outer directory parsing with no slashes failed on: " + platform + "; value was: " + outer_dir);
  
  //  test empty string
  std::string file_path_4;
  
  outer_dir = eowu::path::get_outer_directory(file_path_4);
  
  EOWU_ASSERT_TRUE(outer_dir == "", "Outer directory parsing with empty string succeeded on: " + platform,
                   "Outer directory parsing with empty string failed on: " + platform + "; value was: " + outer_dir);
}
