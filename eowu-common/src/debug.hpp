//
//  debug.hpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace debug {
    void print_error(const std::string &message);
    
    namespace colors {
      extern const char* const red;
      extern const char* const green;
      extern const char* const yellow;
      extern const char* const dflt;
    }
  }
}
