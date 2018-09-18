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
#include <vector>

namespace eowu {
  class State;
  class StateRunner;
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
      
      SharedState(const eowu::Timer *task_timer);
    };
    
    void render(eowu::thread::SharedState &state,
                std::shared_ptr<eowu::LuaContext> lua_context,
                std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions,
                std::shared_ptr<eowu::GLPipeline> pipeline);
    
    bool try_call_render(const std::shared_ptr<eowu::LuaContext> &lua_context,
                         eowu::LuaFunction *render_function);
    
    bool try_call_flip(const std::shared_ptr<eowu::LuaContext> &lua_context,
                       eowu::LuaFunction *flip_function,
                       const eowu::WindowContainerType &gl_windows,
                       const eowu::time::RenderTiming &render_time);
    
    void task(eowu::thread::SharedState &state,
              eowu::StateRunner &state_runner,
              const std::vector<std::shared_ptr<eowu::XYTarget>> &targets);
    
    bool try_update_task(eowu::StateRunner &state_runner);
    
    bool try_update_targets(const std::vector<std::shared_ptr<eowu::XYTarget>> &targets);
  }
}
