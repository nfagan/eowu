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
  class Window;
  class AudioContext;
  
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
              const eowu::TimeoutAggregateMapType *timeouts,
              const eowu::TimeoutAggregateMapType *intervals);
    
    bool try_update_task(eowu::StateRunner &state_runner);
    void try_update_targets(const std::vector<std::shared_ptr<eowu::XYTarget>> &targets);
    void try_update_timeouts(const eowu::TimeoutAggregateMapType *timeouts, const char *const kind);
    
    void events(eowu::thread::SharedState &state,
                std::shared_ptr<eowu::ContextManager> context_manager,
                std::shared_ptr<eowu::AudioContext> audio_context,
                int stop_key_code);
    
    bool try_await_thread_finish(const eowu::thread::SharedState &state,
                              eowu::time::DurationType timeout);
    
    void print_error(const std::string &thread_type, const std::string &message);
  }
}
