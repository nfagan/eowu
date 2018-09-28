//
//  TimeoutWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/19/18.
//

#pragma once

#include "LuaFunction.hpp"
#include <eowu-common/Timeout.hpp>
#include <eowu-common/time.hpp>
#include <eowu-common/LockedResource.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

namespace eowu {
  class LuaContext;
  
  class TimeoutWrapper;
  
  using TimeoutWrapperMapType = std::unordered_map<std::string, std::unique_ptr<eowu::TimeoutWrapper>>;
  using TimeoutWrapperLockedResourceType = eowu::LockedResource<TimeoutWrapperMapType, std::recursive_mutex>;
  using TimeoutWrapperContainerType = std::shared_ptr<TimeoutWrapperLockedResourceType>;
}

class eowu::TimeoutWrapper {
public:
  TimeoutWrapper(std::shared_ptr<eowu::LuaContext> lua_context,
                 const luabridge::LuaRef &func,
                 const eowu::time::DurationType &duration);
  
  ~TimeoutWrapper() = default;
  
  void Update();
  void Cancel();
  
  static void CreateLuaSchema(lua_State *L);
private:
  eowu::Timeout timeout;
  std::shared_ptr<eowu::LuaContext> lua_context;
  eowu::LuaFunction on_ellapsed;
  
  void configure_timeout();
};
