//
//  debug.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#include "debug.hpp"
#include <iostream>

void eowu::debug::print_error(const std::string &message) {
  std::cout << "\033[1;31mERROR: \033[0m" << message;
}
