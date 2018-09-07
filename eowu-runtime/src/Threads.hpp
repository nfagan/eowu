//
//  Threads.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <atomic>
#include <memory>

namespace eowu {
  class State;
  class LuaContext;
  class GLPipeline;
  class Window;
  
  namespace thread {
    struct SharedState {
      std::atomic<bool> task_thread_initialized;
      std::atomic<bool> render_thread_initialized;
      std::atomic<bool> threads_should_continue;
      
      SharedState();
    };
    
    void render(eowu::thread::SharedState &state,
                std::shared_ptr<eowu::LuaContext> lua_context,
                std::shared_ptr<eowu::GLPipeline> pipeline);
    
    void task(eowu::thread::SharedState &state, eowu::State *first_state);
  }
}
