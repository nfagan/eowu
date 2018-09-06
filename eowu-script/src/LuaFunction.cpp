//
//  LuaFunction.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaFunction.hpp"
#include "LuaNoop.hpp"
#include "Error.hpp"

eowu::LuaFunction::LuaFunction(const eowu::LuaReferenceContainer &ref) : function_reference(ref) {
  //
}

eowu::LuaFunction::LuaFunction(const eowu::LuaFunction &other) :
function_reference(other.function_reference) {
  //
}

void eowu::LuaFunction::Set(const eowu::LuaFunction &other) {
  Set(other.function_reference);
}

void eowu::LuaFunction::Set(const eowu::LuaReferenceContainer &func) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  const auto &ref = func.GetReference();
  
  if (!ref.isFunction()) {
    throw eowu::LuaError("Attempted to assign a non-function lua object.");
  }
  
  function_reference = func;
}

void eowu::LuaFunction::Call() const {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  const auto &ref = function_reference.GetReference();
  
  ref();
}

const eowu::LuaFunction& eowu::LuaFunction::get_no_op(lua_State *L) {
  return eowu::LuaNoop::get_no_op(L);
}
