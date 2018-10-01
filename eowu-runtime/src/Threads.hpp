//
//  Threads.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "Timing.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>
#include <atomic>
#include <memory>
#include <vector>

namespace eowu {
  class State;
  class StateRunner;
  class LuaContext;
  class LuaFunction;
  class LockedLuaRenderFunctions;
  class ContextManager;
  class GLPipeline;
  class Window;
  
  namespace thread {
    struct SharedState {
      std::atomic<bool> assigned_thread_ids;
      std::atomic<bool> task_thread_initialized;
      std::atomic<bool> render_thread_initialized;
      std::atomic<bool> threads_should_continue;
      std::atomic<bool> task_thread_completed;
      std::atomic<bool> render_thread_completed;
      
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
              const std::vector<std::shared_ptr<eowu::XYTarget>> &targets,
              eowu::TimeoutWrapperContainerType timeouts);
    
    bool try_update_task(eowu::StateRunner &state_runner);
    bool try_update_targets(const std::vector<std::shared_ptr<eowu::XYTarget>> &targets);
    bool try_update_timeouts(const eowu::TimeoutWrapperContainerType &timeouts);
    
    void events(eowu::thread::SharedState &state, std::shared_ptr<eowu::ContextManager> context_manager);
    
    bool try_await_thread_finish(const eowu::thread::SharedState &state,
                              eowu::time::DurationType timeout);
    
    void print_error(const std::string &thread_type, const std::string &message);
  }
}
