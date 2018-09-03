//
//  LuaReferenceContainer.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <Lua.hpp>

namespace eowu {
  class LuaReferenceContainer;
}

class eowu::LuaReferenceContainer {
  
public:
  explicit LuaReferenceContainer(const LuaReferenceContainer &other);
  explicit LuaReferenceContainer(const luabridge::LuaRef &ref_);
  
  const luabridge::LuaRef& GetReference() const;
  
private:
  luabridge::LuaRef ref;
};
