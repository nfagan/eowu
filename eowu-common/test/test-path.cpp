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

void eowu::test::test_fullfile() {
  EOWU_PRINT_TEST_BEGIN("full_file");
  
#ifdef EOWU_IS_WIN
  std::vector<std::string> components{"C:\\", "Users\\Nick\\", "Another"};
  std::vector<std::string> components2{"C:\\", "\\\\Users\\Nick\\", "\\Another"};
  std::string expected = "C:\\Users\\Nick\\Another";
#else
  std::vector<std::string> components{"/Users", "Nick/", "Another"};
  std::vector<std::string> components2{"/Users/", "//Nick/", "/Another"};
  std::string expected = "/Users/Nick/Another";
#endif
  
  std::string platform = eowu::platform::name;
  
  auto res = eowu::path::full_file(components);
  EOWU_ASSERT_TRUE(res == expected, "full_file succeeded on: " + platform + "; value was: " + res,
                   "full_file failed on: " + platform + "; value was: " + res);
  
  
  auto res2 = eowu::path::full_file(components2);
  EOWU_ASSERT_TRUE(res2 == expected, "full_file succeeded on: " + platform + "; value was: " + res2,
                   "full_file failed on: " + platform + "; value was: " + res2);
  
}

void eowu::test::test_path() {
  EOWU_PRINT_TEST_BEGIN("path");
  
  std::string platform = eowu::platform::name;
  
#ifndef EOWU_IS_WIN
  std::string file_path_1 = "/Users/Nick/test";
  std::string expected_1 = "/Users/Nick";
#else
  std::string file_path_1 = "C:\\Users\\Nick\\test";
  std::string expected_1 = "C:\\Users\\Nick";
#endif
  
  auto outer_dir = eowu::path::get_outer_directory(file_path_1);
  
  EOWU_ASSERT_TRUE(outer_dir == expected_1, "Outer directory parsing succeeded on: " + platform,
                   "Outer directory parsing failed on: " + platform + "; value was: " + outer_dir);
  
  //  test trailing slash
#ifndef EOWU_IS_WIN
  std::string file_path_2 = "/Users/Nick/test/";
  std::string expected_2 = expected_1;
#else
  std::string file_path_2 = "C:\\Users\\Nick\\test\\";
  std::string expected_2 = expected_1;
#endif
  
  outer_dir = eowu::path::get_outer_directory(file_path_2);
  
  EOWU_ASSERT_TRUE(outer_dir == expected_2, "Outer directory parsing with trailing slash succeeded on: " + platform,
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
