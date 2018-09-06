//
//  Threads.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Threads.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-common/logging.hpp>
#include <stdexcept>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

eowu::thread::SharedState::SharedState() :
task_thread_initialized(false), render_thread_initialized(false), threads_should_continue(true) {
  //
}

void eowu::thread::task(eowu::thread::SharedState &state, eowu::State *first_state) {
  
  eowu::StateManager state_manager;
  eowu::StateRunner state_runner;
  
  state_runner.Begin(first_state);
  
  state.task_thread_initialized.store(true);
  
  while (!state.render_thread_initialized) {
    EOWU_LOG_INFO("Task thread: Awaiting render thread initialization.");
  }
  
  bool should_proceed = true;
  
  while (state.threads_should_continue && should_proceed) {
    try {
      should_proceed = !state_runner.Update();
    } catch (const std::exception &e) {
      std::cout << "ERROR: Task: " << e.what() << std::endl;
      
      should_proceed = false;
    }
  }
  
  state.threads_should_continue = false;
}

void eowu::thread::render(eowu::thread::SharedState &state,
                          std::shared_ptr<eowu::LuaContext> lua_context,
                          std::shared_ptr<eowu::GLPipeline> pipeline) {
  
  auto renderer = pipeline->GetRenderer();
  auto gl_context_manager = pipeline->GetContextManager();
  auto gl_windows = gl_context_manager->GetWindows();
  
  state.render_thread_initialized = true;
  
  while (!state.task_thread_initialized) {
    //
  }
  
  const auto &lua_render_function = *eowu::ScriptWrapper::LuaRenderFunction.get();
  
  while (state.threads_should_continue) {
    if (gl_context_manager->AllShouldClose()) {
      break;
    }
    
    try {
      lua_context->Call(lua_render_function);
      
    } catch (const std::exception &e) {
      std::cout << "ERROR: Render: " << e.what() << std::endl;
      
      state.threads_should_continue = false;
      break;
    }
    
    renderer->Draw();
    renderer->ClearQueue();
    gl_context_manager->PollEvents();
  }
  
  gl_context_manager->CloseWindows();
  
  state.threads_should_continue = false;
}
