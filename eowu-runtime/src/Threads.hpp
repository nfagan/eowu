//
//  Threads.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "Timing.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <atomic>
#include <memory>

namespace eowu {
  class State;
  class LuaContext;
  class LuaFunction;
  class LockedLuaRenderFunctions;
  class GLPipeline;
  class Window;
  
  namespace thread {
    struct SharedState {
      std::atomic<bool> task_thread_initialized;
      std::atomic<bool> render_thread_initialized;
      std::atomic<bool> threads_should_continue;
      
      eowu::time::Timing timing;
      
      SharedState();
    };
    
    void render(eowu::thread::SharedState &state,
                std::shared_ptr<eowu::LuaContext> lua_context,
                eowu::LockedLuaRenderFunctions &lua_render_functions,
                std::shared_ptr<eowu::GLPipeline> pipeline);
    
    bool try_call_render(const std::shared_ptr<eowu::LuaContext> &lua_context,
                         eowu::LuaFunction *render_function);
    
    bool try_call_flip(const std::shared_ptr<eowu::LuaContext> &lua_context,
                       eowu::LuaFunction *flip_function,
                       const eowu::WindowContainerType &gl_windows,
                       const eowu::time::RenderTiming &render_time);
    
    void task(eowu::thread::SharedState &state, eowu::State *first_state);
  }
}
