//
//  Threads.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Threads.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-common/logging.hpp>
#include <eowu-common/config.hpp>
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
  const auto &state_runner_timer = state_runner.GetTimer();
  
  state_runner.Begin(first_state);
  
  state.task_thread_initialized.store(true);
  
  while (!state.render_thread_initialized) {
    EOWU_LOG_INFO("Task thread: Awaiting render thread initialization.");
  }
  
  bool should_proceed = true;
  
  while (state.threads_should_continue && should_proceed) {
    
    //  try to call the lua function (entry, loop, or exit)
    //  for this frame, and abort on error in the script.
    try {
      should_proceed = !state_runner.Update();
    } catch (const std::exception &e) {
      std::cout << "ERROR: Task: " << e.what() << std::endl;
      
      should_proceed = false;
    }
    
    double ellapsed = state_runner_timer.Ellapsed().count();
    state.timing.task.Update(ellapsed);
  }
  
  state.threads_should_continue = false;
}

void eowu::thread::render(eowu::thread::SharedState &state,
                          std::shared_ptr<eowu::LuaContext> lua_context,
                          eowu::LockedLuaRenderFunctions &lua_render_functions,
                          std::shared_ptr<eowu::GLPipeline> pipeline) {
  
  auto renderer = pipeline->GetRenderer();
  auto gl_context_manager = pipeline->GetContextManager();
  auto gl_windows = gl_context_manager->GetWindows();
  auto &render_time = state.timing.render;
  auto &task_time = state.timing.task;
  
  state.render_thread_initialized = true;
  
  //  wait for task thread to begin. this can theoretically
  //  fail, but if so the task won't have started, so all
  //  should be ok anyway.
  while (!state.task_thread_initialized) {
    //
  }
  
  //  update the time relative to the task time
  renderer->SetOnBufferSwap([&](auto &win) -> void {
    render_time.SetCurrentTime(win->GetAlias(), task_time.GetCurrentTime());
  });
  
  //  main loop
  while (state.threads_should_continue) {
    //  if all windows are closed, exit the render thread.
    if (gl_context_manager->AllShouldClose()) {
      break;
    }
    
    lua_render_functions.Use([&](auto *render, auto *flip) -> void {
      //  call render function for this frame
      bool render_res = eowu::thread::try_call_render(lua_context, render);
      
      if (!render_res) {
        state.threads_should_continue = false;
        return;
      }
      
      //  draw, empty queue, update window events
      renderer->Draw();
      renderer->ClearQueue();
      gl_context_manager->PollEvents();
      
      //  call flip function for this frame
      bool flip_res = eowu::thread::try_call_flip(lua_context, flip, gl_windows, render_time);
      
      if (!flip_res) {
        state.threads_should_continue = false;
        return;
      }
    });
  }
  
  //  clean-up
  lua_render_functions.Flush();
  
  gl_context_manager->CloseWindows();
  
  state.threads_should_continue = false;
}

bool eowu::thread::try_call_flip(const std::shared_ptr<eowu::LuaContext> &lua_context,
                                 eowu::LuaFunction *flip_function,
                                 const eowu::WindowContainerType &gl_windows,
                                 const eowu::time::RenderTiming &render_time) {
  if (flip_function == nullptr) {
    //  flip function will be null until explicitly assigned in the user's script.
    return true;
  }
  
  bool success;
  
  try {
    //  for each window, check whether we've flipped
    //  the screen for this window yet. if not, return early.
    //  otherwise, call the current flip() function with the current
    //  alias and timestamp
    for (const auto &win : gl_windows) {
      const std::string &alias = win->GetAlias();
      
      if (!win->IsOpen() || !render_time.HasAlias(alias)) {
        continue;
      }
      
      lua_context->Call(*flip_function, alias, render_time.GetCurrentTime(alias));
    }
    
    success = true;
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: Flip: " << e.what() << std::endl;
    
    success = false;
  }
  
  return success;
}

bool eowu::thread::try_call_render(const std::shared_ptr<eowu::LuaContext> &lua_context,
                                   eowu::LuaFunction *render_function) {
  if (render_function == nullptr) {
    //  render function will be null until explicitly assigned in the user's script.
    return true;
  }
  
  bool success;
  
  try {
    lua_context->Call(*render_function);
    success = true;
    
  } catch (const std::exception &e) {
    std::cout << "ERROR: Render: " << e.what() << std::endl;
    
    success = false;
  }
  
  return success;
}
