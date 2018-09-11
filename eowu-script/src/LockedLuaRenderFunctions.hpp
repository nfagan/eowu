//
//  LuaRenderThreadFunctions.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/10/18.
//

#pragma once

#include "LuaFunction.hpp"
#include <mutex>
#include <functional>
#include <atomic>

namespace eowu {
  class LockedLuaRenderFunctions;
}

class eowu::LockedLuaRenderFunctions {
public:
  LockedLuaRenderFunctions(eowu::LuaFunction *render, eowu::LuaFunction *flip);
  ~LockedLuaRenderFunctions() = default;
  
  void Queue(eowu::LuaFunction *render, eowu::LuaFunction *flip);
  void Use(const std::function<void(eowu::LuaFunction*, eowu::LuaFunction*)> &cb);
  void Flush();
  
  bool IsQueued() const;
  bool DidUse() const;
  
private:
  mutable std::mutex mut;
  
  eowu::LuaFunction *render_function;
  eowu::LuaFunction *flip_function;
  
  eowu::LuaFunction *queued_render_function;
  eowu::LuaFunction *queued_flip_function;
  
  std::atomic<bool> did_use;
  std::atomic<bool> is_queued;
};
