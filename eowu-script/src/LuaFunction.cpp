//
//  LuaFunction.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaFunction.hpp"
#include "LuaNoop.hpp"
#include "Error.hpp"

eowu::LuaFunction::LuaFunction(const luabridge::LuaRef &ref) :
did_call(false), function_reference(ref) {
  //
}

eowu::LuaFunction::LuaFunction(const eowu::LuaFunction &other) :
did_call(other.did_call.load()), function_reference(other.GetReference()) {
  //
}

void eowu::LuaFunction::Set(const eowu::LuaFunction &other) {
  std::unique_lock<std::recursive_mutex> lock(other.mut);
  
  Set(other.function_reference);
}

const luabridge::LuaRef& eowu::LuaFunction::GetReference() const {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  return function_reference;
}

bool eowu::LuaFunction::DidCall() const {
  return did_call;
}

void eowu::LuaFunction::Set(const luabridge::LuaRef &ref) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  if (!ref.isFunction()) {
    throw eowu::LuaError("Attempted to assign a non-function lua object.");
  }
  
  function_reference = ref;
  
  did_call = false;
}

bool eowu::LuaFunction::IsValid() const {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  bool res = function_reference.isFunction();
  
  return res;
}

void eowu::LuaFunction::AbortCall() {
  did_call = true;
}

const eowu::LuaFunction& eowu::LuaFunction::get_no_op(lua_State *L) {
  return eowu::LuaNoop::get_no_op(L);
}
