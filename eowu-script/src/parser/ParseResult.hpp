//
//  ParseResult.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace parser {
    template<typename T>
    struct ParseResult;
  }
}

template<typename T>
struct eowu::parser::ParseResult {
  
  ParseResult() {
    success = false;
  }
  
  ~ParseResult() = default;
  
  bool success;
  std::string message;
  T result;
};
