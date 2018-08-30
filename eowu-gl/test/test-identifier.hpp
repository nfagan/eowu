//
//  test-identifier.hpp
//  eowu
//
//  Created by Nick Fagan on 8/30/18.
//

#pragma once

#include <eowu-gl/eowu-gl.hpp>

void test_non_matching() {
  eowu::Identifier id1;
  eowu::Identifier id2;
  
  assert(id1 != id2);
  assert(!(id1 == id2));
  
  std::cout << "OK: Newly created ids were different" << std::endl;
}

void test_identifier_run_all() {
  std::cout << "--------" << "identifier" << std::endl;
  test_non_matching();
}
