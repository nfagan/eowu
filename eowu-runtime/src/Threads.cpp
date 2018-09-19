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
#include <cstddef>
#include <chrono>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

eowu::thread::SharedState::SharedState(const eowu::Timer *task_timer) :
assigned_thread_ids(false), task_thread_initialized(false), render_thread_initialized(false),
threads_should_continue(true), task_thread_completed(false), render_thread_completed(false),
timing(task_timer) {
  //
}

void eowu::thread::task(eowu::thread::SharedState &state,
                        eowu::StateRunner &state_runner,
                        const std::vector<std::shared_ptr<eowu::XYTarget>> &targets) {
  
  state.task_thread_initialized.store(true);
  
  //  Wait for the render thread to initialize and for thread ids to be
  //  assigned to the script wrapper.
  while (!state.render_thread_initialized || !state.assigned_thread_ids) {
    //
  }
  
  bool should_proceed = true;
  
  while (state.threads_should_continue && should_proceed) {
    
    //  If the current state will exit on this frame, update the targets
    //  before calling the user-supplied exit() function. Otherwise, update
    //  the targets *after* the user-supplied entry() or loop() functions.
    bool active_state_will_exit = state_runner.ActiveStateWillExit();
    
    if (active_state_will_exit) {
      should_proceed = eowu::thread::try_update_targets(targets);
    }
    
    should_proceed = eowu::thread::try_update_task(state_runner);
    
    if (should_proceed && !active_state_will_exit) {
      should_proceed = eowu::thread::try_update_targets(targets);
    }
  }
  
  state.threads_should_continue = false;
  state.task_thread_completed = true;
}

bool eowu::thread::try_update_task(eowu::StateRunner &state_runner) {
  //  try to call the lua function (entry, loop, or exit)
  //  for this frame, and abort on error in the script.
  
  //  Note that the state runner timer is updated in Update().
  bool should_proceed;
  
  try {
    should_proceed = !state_runner.Update();
    
  } catch (const std::exception &e) {
    eowu::thread::print_error("TASK", e.what());
    
    should_proceed = false;
  }
  
  return should_proceed;
}

bool eowu::thread::try_update_targets(const std::vector<std::shared_ptr<eowu::XYTarget>> &targets) {
  std::size_t sz = targets.size();
  
  for (std::size_t i = 0; i < sz; i++) {
    const auto &targ = targets[i];
    
    try {
      targ->Update();
    } catch (const std::exception &e) {
      eowu::thread::print_error("TARGET", e.what());
      
      return false;
    }
  }
  
  return true;
}

void eowu::thread::render(eowu::thread::SharedState &state,
                          std::shared_ptr<eowu::LuaContext> lua_context,
                          std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions,
                          std::shared_ptr<eowu::GLPipeline> pipeline) {
  
  auto renderer = pipeline->GetRenderer();
  auto gl_context_manager = pipeline->GetContextManager();
  auto gl_windows = gl_context_manager->GetWindows();
  auto &render_time = state.timing.render;
  const auto *task_timer = state.timing.task;
  
  state.render_thread_initialized = true;
  
  //  wait for task thread to begin and for thread ids to be assigned.
  //  this can theoretically fail, but if so the task won't have started.
  while (!state.task_thread_initialized || !state.assigned_thread_ids) {
    //
  }
  
  //  update the time relative to the task time
  renderer->SetOnBufferSwap([&](auto &win) -> void {
    render_time.SetCurrentTime(win->GetAlias(), task_timer->Ellapsed().count());
  });
  
  //  main loop
  while (state.threads_should_continue) {
    //  if all windows are closed, exit the render thread.
    if (gl_context_manager->AllShouldClose()) {
      break;
    }
    
    lua_render_functions->Use([&](auto *render, auto *flip) -> void {
      //  call render function for this frame
      bool render_res = eowu::thread::try_call_render(lua_context, render);
      
      if (!render_res) {
        state.threads_should_continue = false;
        return;
      }
      
      //  draw, empty queue
      renderer->Draw();
      renderer->ClearQueue();
      
      //  call flip function for this frame
      bool flip_res = eowu::thread::try_call_flip(lua_context, flip, gl_windows, render_time);
      
      if (!flip_res) {
        state.threads_should_continue = false;
        return;
      }
    });
  }
  
  //  clean-up
  lua_render_functions->Flush();
  
  gl_context_manager->CloseWindows();
  
  state.threads_should_continue = false;
  state.render_thread_completed = true;
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
    eowu::thread::print_error("FLIP", e.what());
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
    eowu::thread::print_error("RENDER", e.what());
    success = false;
  }
  
  return success;
}

void eowu::thread::events(eowu::thread::SharedState &state, std::shared_ptr<eowu::ContextManager> context_manager) {
  int key_code = eowu::Keyboard::GetKeyCode("escape");
  auto &kb = context_manager->GetKeyboard();
  
  //  main events loop
  while (state.threads_should_continue && !context_manager->AllShouldClose()) {
    context_manager->PollEvents();
    
    if (kb.IsPressed(key_code)) {
      state.threads_should_continue = false;
    }
  }
}

void eowu::thread::try_await_thread_finish(const eowu::thread::SharedState &state,
                                           eowu::time::DurationType timeout) {
  
  eowu::Timer timer;
  bool threads_completed = state.render_thread_completed && state.task_thread_completed;
  bool did_warn = false;
  
  while (!threads_completed && timer.Ellapsed() < timeout) {
    threads_completed = state.render_thread_completed && state.task_thread_completed;
    timer.Update();
    
    if (!did_warn && timer.Ellapsed() >= std::chrono::seconds(1)) {
      std::cout << "WARN: More than 1 second ellapsed waiting for render and/or task threads to complete." << std::endl;
      did_warn = true;
    }
  }
  
  if (!threads_completed) {
    std::chrono::duration<double, std::milli> ms = timeout;
    std::string t = std::to_string(ms.count());
    
    std::string base_msg = "The render and/or task threads failed to complete in: " + t + " ms.";
    std::string rest_msg = "Check your script(s) to see whether your code includes an infinite while loop. Terminating ...";
    std::string full_msg = base_msg + " " + rest_msg;
    
    throw std::runtime_error(full_msg);
  }
}

void eowu::thread::print_error(const std::string &thread_type, const std::string &message) {
  const std::string base = "\033[1;31mERROR [" + thread_type + "]: \033[0m";
  std::cout << std::endl << base << message << std::endl << std::endl;
}
