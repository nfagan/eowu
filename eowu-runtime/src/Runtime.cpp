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
#include <eowu-common/path.hpp>
#include <Lua.hpp>
#include <iostream>
#include <thread>

int eowu::Runtime::Main(const std::string &file) {
  eowu::ScriptWrapper script_wrapper;
  eowu::StateManager state_manager;
  eowu::StateRunner state_runner;
  
  eowu::LuaRuntime lua_runtime(state_manager, state_runner);
  
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
  script_wrapper.SetTaskDataStore(task_data_store);
  const auto &data_path = lua_runtime.setup_schema.paths.data;
  
  if (!eowu::path::directory_exists(data_path)) {
    throw std::runtime_error("Directory: '" + data_path + "' does not exist.");
  }
  
  const auto data_file = eowu::path::full_file({data_path, "task.dat"});
  task_data_store->Open(data_file);
  //  end task data store
  
  //  @TODO: Handle task-data storage here as well.
  lua_runtime.InitializeScriptWrapper(script_wrapper, file, gl_pipeline);
  
  eowu::thread::SharedState thread_state;
  
  //  Task thread
  state_runner.Begin(lua_runtime.GetFirstState());
  
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state), std::ref(state_runner));
  
  //  Render thread
  auto locked_lua_functions = script_wrapper.GetLockedRenderFunctions();
  const auto &lua_render_context = lua_runtime.lua_contexts.render;
  
  eowu::thread::render(thread_state, lua_render_context, locked_lua_functions, gl_pipeline);
  
  task_thread.join();
  
  return 0;
}
