//
//  LuaFunction.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "LuaFunction.hpp"
#include "LuaNoop.hpp"
#include "Error.hpp"

eowu::LuaFunction::LuaFunction(lua_State *L) :
did_call(false), function_reference(get_no_op(L).GetReference()) {
  //
}

eowu::LuaFunction::LuaFunction(const luabridge::LuaRef &ref) :
did_call(false), function_reference(ref) {
  //
}

eowu::LuaFunction::LuaFunction(const eowu::LuaFunction &other) :
did_call(other.did_call.load()), function_reference(other.GetReference()) {
  //
}

eowu::LuaFunction& eowu::LuaFunction::operator=(const eowu::LuaFunction &other) {
  did_call = other.did_call.load();
  function_reference = other.GetReference();
  
  return *this;
}

void eowu::LuaFunction::Set(const eowu::LuaFunction &other) {
  Set(other.function_reference);
}

const luabridge::LuaRef& eowu::LuaFunction::GetReference() const {
  return function_reference;
}

bool eowu::LuaFunction::DidCall() const {
  return did_call;
}

void eowu::LuaFunction::Set(const luabridge::LuaRef &ref) {
  if (!ref.isFunction()) {
    throw eowu::LuaError("Attempted to assign a non-function lua object.");
  }
  
  function_reference = ref;
  
  did_call = false;
}

bool eowu::LuaFunction::IsValid() const {
  return function_reference.isFunction();
}

void eowu::LuaFunction::AbortCall() {
  did_call = true;
}

const eowu::LuaFunction& eowu::LuaFunction::get_no_op(lua_State *L) {
  return eowu::LuaNoop::get_no_op(L);
}
