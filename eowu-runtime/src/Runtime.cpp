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
#include <eowu-data.hpp>
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
  
  //  task data store
  auto task_data_store = std::make_shared<eowu::data::Store>();
  eowu::ScriptWrapper::task_data_store = task_data_store;
  
  const auto &data_file = lua_runtime.setup_schema.paths.data + "task.dat";
  task_data_store->Open(data_file);
  //  end task data store
  
  lua_runtime.InitializeScriptWrapper(file, gl_pipeline);
  
  eowu::thread::SharedState thread_state;
  
  auto first_state = lua_runtime.GetFirstState();
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state), first_state);
  
  auto &locked_lua_functions = *eowu::ScriptWrapper::LuaRenderThreadFunctions.get();
  const auto &render_context = lua_runtime.lua_contexts.render;
  
  eowu::thread::render(thread_state, render_context, locked_lua_functions, gl_pipeline);
  
  task_thread.join();
  
  return 0;
}
