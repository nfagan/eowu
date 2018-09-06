//
//  LuaFunction.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include "LuaReferenceContainer.hpp"
#include <Lua.hpp>
#include <mutex>

namespace eowu {
  class LuaFunction;
  class LuaReferenceContainer;
}

class eowu::LuaFunction {
public:
  LuaFunction(const eowu::LuaReferenceContainer &func);
  LuaFunction(const eowu::LuaFunction &other);
  
  void Set(const eowu::LuaFunction &other);
  void Set(const eowu::LuaReferenceContainer &func);
  void Call() const;
  
private:
  mutable std::recursive_mutex mut;
  
  eowu::LuaReferenceContainer function_reference;
};
