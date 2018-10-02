//
//  debug.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#include "debug.hpp"
#include <iostream>

void eowu::debug::print_error(const std::string &message) {
  std::cout << colors::red << "ERROR: " << colors::dflt << message;
}

const char* const eowu::debug::colors::red = "\x1B[31m";
const char* const eowu::debug::colors::green = "\x1B[32m";
const char* const eowu::debug::colors::yellow = "\x1B[33m";
const char* const eowu::debug::colors::dflt = "\x1B[0m";
