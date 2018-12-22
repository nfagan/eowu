//
//  Runtime.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <memory>

struct lua_State;

namespace eowu {
  namespace runtime {
    int main(const std::string &file);
  }
}
