//
//  TimeoutWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#pragma once

#include "LuaFunction.hpp"
#include <eowu-common/Timeout.hpp>

namespace eowu {
  class TimeoutWrapper;
}

class eowu::TimeoutWrapper {
public:
  TimeoutWrapper() = default;
  ~TimeoutWrapper() = default;
  
  void Update();
  void Reset();
  
  static void CreateLuaSchema(lua_State *L);
private:
  eowu::Timeout timeout;
  eowu::LuaFunction on_ellapsed;
};
