//
//  LuaReferenceContainer.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaReferenceContainer.hpp"

eowu::LuaReferenceContainer::LuaReferenceContainer(const LuaReferenceContainer &other) : ref(other.GetReference()) {
  //
}
  
eowu::LuaReferenceContainer::LuaReferenceContainer(const luabridge::LuaRef &ref_) : ref(ref_) {
  //
}
  
const luabridge::LuaRef& eowu::LuaReferenceContainer::GetReference() const {
  return ref;
}
