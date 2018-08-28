//
//  Error.hpp
//  eowu
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include <stdexcept>
#include <string>

namespace eowu {
  class NonMatchingVerticesError : public std::runtime_error {
  public:
    NonMatchingVerticesError(const std::string &msg) : std::runtime_error(msg) {}
  };
}
