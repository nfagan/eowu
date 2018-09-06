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
  
  if (!lua_runtime.InitializeSchema(file)) {
    return 1;
  }
  
  auto gl_pipeline = eowu::GLPipeline::GetInstance();
  auto gl_status = init::initialize_gl_pipeline(gl_pipeline, lua_runtime.setup_schema);
  
  if (!gl_status.success) {
    gl_status.file = file;
    gl_status.print();
    return 1;
  }
  
  lua_runtime.InitializeScriptWrapper(file, gl_pipeline);
  
  eowu::thread::SharedState thread_state;
  
  auto first_state = lua_runtime.GetFirstState();
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state), first_state);
  
  eowu::thread::render(thread_state, lua_runtime.lua_contexts.render, gl_pipeline);
  
  task_thread.join();
  
  return 0;
}
