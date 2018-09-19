//
//  Runtime.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "LuaRuntime.hpp"
#include <eowu-common/time.hpp>
#include <string>
#include <memory>

struct lua_State;

namespace eowu {
  class Runtime;
  class GLPipeline;
  
  namespace thread {
    struct SharedState;
  }
}

class eowu::Runtime {
public:
  Runtime() = default;
  ~Runtime() = default;
  
  int Main(const std::string &file);
};
