//
//  test-parser.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#include "test-parser.hpp"
#include "test-window-parser.hpp"
#include "test-stimuli-parse.hpp"
#include "test-state-parse.hpp"
#include <iostream>

void test_setup_parser_run_all() {
  test_window();
  test_stimuli_run_all();
  test_state_parse_run_all();
}

void test_parser_run_all() {
  std::cout << "--------" << "parser" << std::endl;
  
  test_setup_parser_run_all();
}
