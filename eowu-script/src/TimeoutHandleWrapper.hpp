//
//  TimeoutHandleWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/17/18.
//

#pragma once

#include <atomic>
#include <memory>

struct lua_State;

namespace eowu {
  class TimeoutHandleWrapper;
  class TimeoutHandle;
}

class eowu::TimeoutHandleWrapper {
public:
  TimeoutHandleWrapper() = default;
  TimeoutHandleWrapper(std::shared_ptr<eowu::TimeoutHandle> timeout_handle);
  ~TimeoutHandleWrapper() = default;
  
  void Cancel();
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::TimeoutHandle> timeout_handle;
};
