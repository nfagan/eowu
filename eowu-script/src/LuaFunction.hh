//
//  LuaFunction.hh
//  eowu
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

#include "Error.hpp"

template<typename ...Args>
void eowu::LuaFunction::Call(Args... args) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  try {
    function_reference(args...);
    did_call = true;
  } catch (const std::exception &e) {
    did_call = true;
    throw eowu::LuaError(e.what());
  }
}
