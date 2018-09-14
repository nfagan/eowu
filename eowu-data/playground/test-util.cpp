//
//  test-util.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "test-util.hpp"
#include <eowu-common/path.hpp>
#include <assert.h>

std::string eowu::test::util::get_test_data_dir() {
  bool success;
  
  const auto eowu_root = eowu::path::get_eowu_root_directory(&success);
  
  assert(success);
  
  return eowu_root + "/eowu-data/data";
}
