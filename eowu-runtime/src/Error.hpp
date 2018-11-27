//
//  Error.hpp
//  eowu
//
//  Created by Nick Fagan on 11/17/18.
//

#pragma once

#include <stdexcept>

namespace eowu {
  class RuntimeError;
}

class eowu::RuntimeError : public std::runtime_error {
public:
  RuntimeError(const std::string &context, const std::string &msg) : std::runtime_error(msg), context(context) {}
  
  std::string context;
};
