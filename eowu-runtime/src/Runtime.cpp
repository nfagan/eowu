//
//  Runtime.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Runtime.hpp"
#include "GLInit.hpp"
#include "Threads.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <Lua.hpp>
#include <iostream>
#include <thread>

int eowu::Runtime::Main(const std::string &file) {
  
  if (!lua_runtime.Initialize(file)) {
    return 1;
  }
  
  auto gl_pipeline = eowu::GLPipeline::GetInstance();
  auto gl_status = init::initialize_gl_pipeline(gl_pipeline, lua_runtime.setup_schema);
  
  if (!gl_status.success) {
    gl_status.file = file;
    gl_status.print();
    return 1;
  }
  
  lua_runtime.script_wrapper.SetGLPipeline(gl_pipeline);
  
  eowu::thread::SharedState thread_state;
  
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state), lua_runtime.state_manager.GetState("1"));
  
  auto win = gl_pipeline->GetContextManager()->GetWindows()[0];
  
  eowu::thread::render(thread_state, lua_runtime.lua_contexts.render, gl_pipeline, win);
  
  task_thread.join();
  
  return 0;
}
