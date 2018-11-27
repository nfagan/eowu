//
//  test-util.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "test-util.hpp"
#include <eowu-common/fs.hpp>
#include <assert.h>

std::string eowu::test::util::get_test_data_dir() {
  const auto eowu_root = eowu::fs::get_eowu_root_directory();  
  return eowu_root + "/eowu-data/data";
}
