//
//  test-directory-contents.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/13/18.
//

#include "test-directory-contents.hpp"

#include <eowu-common/fs.hpp>
#include <eowu-common/test.hpp>
#include <eowu-common/platform.hpp>
#include <iostream>

namespace priv {
  void print_file_info(const std::vector<eowu::fs::finfo> &info) {
    for (std::size_t i = 0; i < info.size(); i++) {
      std::cout << info[i].name << std::endl;
    }
  }
  
  std::string get_message_wrong_files(const std::string &dir, std::size_t expected, std::size_t actual) {
    return ("Expected " + std::to_string(expected) + " files in: " +
      dir + "; found: " + std::to_string(actual));
  }
  
  std::string get_message_correct_files(const std::string &dir, std::size_t expected) {
    return "Found " + std::to_string(expected) + " files in: " + dir;
  }
  
  std::size_t get_count_no_ds_store(const std::vector<eowu::fs::finfo> &info) {
    std::size_t amt = 0;
    
    for (std::size_t i = 0; i < info.size(); i++) {
      if (info[i].name != ".DS_Store") {
        amt++;
      }
    }
    
    return amt;
  }
}

void eowu::test::test_directory_contents() {
  EOWU_PRINT_TEST_BEGIN("directory_contents: " + std::string(eowu::platform::name));
  
  bool success = false;
  
  auto eowu_root = eowu::fs::get_eowu_root_directory();
  auto test_dir = eowu::fs::full_file({eowu_root, "eowu-common", "test", "res"});
  auto info = eowu::fs::get_directory_contents(test_dir, false, &success);
  
  //  non recursive
  
  EOWU_ASSERT_TRUE(success,
                   "Obtaining directory contents succeeded.",
                   "Failed to get directory contents from directory.");
  
  const std::size_t expected1 = 3;
  const std::size_t actual1 = priv::get_count_no_ds_store(info);
  
  if (actual1 != expected1) {
    priv::print_file_info(info);
  }
  
  EOWU_ASSERT_TRUE(actual1 == expected1,
                   priv::get_message_correct_files(test_dir, expected1),
                   priv::get_message_wrong_files(test_dir, expected1, actual1));
  
  //  recursive
  
  info = eowu::fs::get_directory_contents(test_dir, true, &success);
  EOWU_ASSERT_TRUE(success,
                   "Obtaining directory contents recursively succeeded.",
                   "Failed to get directory contents recursively from directory.");
  
  const std::size_t expected2 = 5;
  const std::size_t actual2 = priv::get_count_no_ds_store(info);
  
  EOWU_ASSERT_TRUE(actual2 == expected2,
                   priv::get_message_correct_files(test_dir, expected2),
                   priv::get_message_wrong_files(test_dir, expected2, actual2));
  
  if (actual2 != expected2) {
    priv::print_file_info(info);
  }
  
  //  fail
  auto nonexistent_test_dir = eowu::fs::full_file({test_dir, "nonexistent"});
  
  info = eowu::fs::get_directory_contents(nonexistent_test_dir, false, &success);
  
  EOWU_ASSERT_TRUE(!success, "Nonexistent directory failed to succeed.",
                   "Nonexistent directory was processed successfully.");
  EOWU_ASSERT_TRUE(priv::get_count_no_ds_store(info) == 0, "Nonexistent directory had length = 0",
                   priv::get_message_wrong_files(nonexistent_test_dir, 0, info.size()));
  
}
