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
    
    void handle_flip(const eowu::WindowContainerType &gl_windows,
                     eowu::time::RenderTiming &render_time,
                     const std::shared_ptr<eowu::LuaContext> &lua_context,
                     eowu::LuaFunction &lua_flip_function);
    
    void render(eowu::thread::SharedState &state,
                std::shared_ptr<eowu::LuaContext> lua_context,
                eowu::LuaFunction &lua_render_function,
                eowu::LuaFunction &lua_flip_function,
                std::shared_ptr<eowu::GLPipeline> pipeline);
    
    void task(eowu::thread::SharedState &state, eowu::State *first_state);
  }
}
