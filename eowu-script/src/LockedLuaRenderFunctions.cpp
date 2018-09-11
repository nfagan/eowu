//
//  LuaRenderThreadFunctions.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/10/18.
//

#include "LockedLuaRenderFunctions.hpp"
#include "Error.hpp"
#include <eowu-common/logging.hpp>

eowu::LockedLuaRenderFunctions::LockedLuaRenderFunctions(eowu::LuaFunction *render,
                                                         eowu::LuaFunction *flip) :
render_function(render), flip_function(flip),
queued_render_function(render_function), queued_flip_function(flip_function), did_use(false), is_queued(false) {
  //
}

bool eowu::LockedLuaRenderFunctions::IsQueued() const {
  return is_queued;
}

bool eowu::LockedLuaRenderFunctions::DidUse() const {
  return did_use;
}

void eowu::LockedLuaRenderFunctions::Flush() {
  std::unique_lock<std::mutex> lock(mut);
  
  is_queued = false;
  did_use = true;
  
  render_function->AbortCall();
  flip_function->AbortCall();
}

void eowu::LockedLuaRenderFunctions::Queue(eowu::LuaFunction *render, eowu::LuaFunction *flip) {
  while (is_queued) {
    //
  }
  
  queued_render_function = render;
  queued_flip_function = flip;
  
  is_queued = true;
}

void eowu::LockedLuaRenderFunctions::Use(const std::function<void(eowu::LuaFunction*, eowu::LuaFunction*)> &cb) {
  std::unique_lock<std::mutex> lock(mut);
  
  if (is_queued && did_use) {    
    render_function = queued_render_function;
    flip_function = queued_flip_function;
    
    did_use = false;
    is_queued = false;
  }
  
  try {
    cb(render_function, flip_function);
    did_use = true;
  } catch (const std::exception &e) {
    did_use = true;
    
    throw eowu::LuaError(e.what());
  }
}
