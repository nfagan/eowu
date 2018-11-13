//
//  string.hpp
//  eowu-common
//
//  Created by Nick Fagan on 10/3/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace util {
    template<typename T>
    std::string join(const T &v, const char* const delim);
    bool ends_with(const std::string &str, const std::string &end);
  }
}

#include "string.hh"
