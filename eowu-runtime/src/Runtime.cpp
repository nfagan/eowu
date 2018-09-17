//
//  Runtime.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Runtime.hpp"
#include "GLInit.hpp"
#include "DataInit.hpp"
#include "SourceInit.hpp"
#include "TargetInit.hpp"
#include "Threads.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <Lua.hpp>
#include <iostream>
#include <thread>

int eowu::Runtime::Main(const std::string &file) {
  eowu::ScriptWrapper script_wrapper;
  eowu::StateManager state_manager;
  eowu::StateRunner state_runner;
  
  eowu::LuaRuntime lua_runtime(state_manager, state_runner);
  
  //  initial file + schema validation
  if (!lua_runtime.InitializeSchema(file)) {
    return 1;
  }
  
  //  task data store
  const auto &data_root_directory = lua_runtime.setup_schema.paths.data;
  const auto &save_schema = lua_runtime.setup_schema.save;
  
  auto data_init_result = eowu::data::initialize_data_pipeline(data_root_directory, save_schema);
  
  if (!data_init_result.status.success) {
    data_init_result.status.file = file;
    data_init_result.status.print();
    return 1;
  }
  
  //  otherwise, the task data store is ok.
  script_wrapper.SetTaskDataStore(data_init_result.result.task_data_store);
  
  //  gl pipeline
  auto gl_pipeline = eowu::GLPipeline::GetInstance();
  auto gl_status = init::initialize_gl_pipeline(gl_pipeline, lua_runtime.setup_schema);
  
  if (!gl_status.success) {
    gl_status.file = file;
    gl_status.print();
    return 1;
  }
  
  //  sources
  auto source_status = eowu::init::initialize_sources(lua_runtime.setup_schema.sources, gl_pipeline);
  
  if (!source_status.status.success) {
    source_status.status.file = file;
    source_status.status.print();
    return 1;
  }
  
  //  targets
  const auto &xy_sources = source_status.result.xy_sources;
  const auto &target_schema = lua_runtime.setup_schema.targets;
  auto target_status = eowu::init::initialize_targets(target_schema, xy_sources, gl_pipeline);
  
  if (!target_status.status.success) {
    target_status.status.file = file;
    target_status.status.print();
    return 1;
  }
  
  //
  //
  //  otherwise, the gl pipeline and sources are ok.
  lua_runtime.InitializeScriptWrapper(script_wrapper, file, gl_pipeline);
  
  eowu::thread::SharedState thread_state;
  
  // Target thread
  auto vec_targets = eowu::validate::get_values(target_status.result.targets);
  auto targets_thread = std::thread(eowu::thread::targets, std::ref(thread_state), std::ref(vec_targets));
  
  //  Task thread
  state_runner.Begin(lua_runtime.GetFirstState());
  
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state), std::ref(state_runner));
  
  //  Render thread
  auto locked_lua_functions = script_wrapper.GetLockedRenderFunctions();
  const auto &lua_render_context = lua_runtime.lua_contexts.render;
  
  eowu::thread::render(thread_state, lua_render_context, locked_lua_functions, gl_pipeline);
  
  task_thread.join();
  targets_thread.join();
  
  return 0;
}
