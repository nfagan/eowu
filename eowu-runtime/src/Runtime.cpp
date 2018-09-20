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
#include <eowu-common/Timer.hpp>
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
  
  //  maps each target to an id.
  const auto &target_map = target_status.result.targets;
  const auto &target_models = target_status.result.target_models;
  const auto &targets_hidden = target_status.result.hidden;

  //  maps each target id to a target wrapper.
  auto target_wrappers = eowu::init::make_target_wrappers(lua_runtime.lua_contexts.task,
                                                          target_map, target_models, targets_hidden, gl_pipeline);
  
  script_wrapper.SetTargetWrapperContainer(target_wrappers);
  script_wrapper.SetXYTargets(target_map);
  
  //
  //
  //  otherwise, the gl pipeline and sources are ok.
  lua_runtime.InitializeScriptWrapper(script_wrapper, file, gl_pipeline);
  
  eowu::thread::SharedState thread_state(&state_runner.GetTimer());
  
  //  Task thread
  auto vec_targets = eowu::validate::get_values(target_map);
  state_runner.Begin(lua_runtime.GetFirstState());
  
  auto task_thread = std::thread(eowu::thread::task, std::ref(thread_state),
                                 std::ref(state_runner), std::ref(vec_targets));

	//	Detach gl-context from main thread.
	auto context_manager = gl_pipeline->GetContextManager();
	context_manager->DetachCurrentContext();
  
  //  Render thread
  auto locked_lua_functions = script_wrapper.GetLockedRenderFunctions();
  const auto &lua_render_context = lua_runtime.lua_contexts.render;
  
  auto render_thread = std::thread(eowu::thread::render, std::ref(thread_state),
                                   lua_render_context, locked_lua_functions, gl_pipeline);
  
  //  Assign thread ids
  script_wrapper.SetThreadIds(render_thread.get_id(), task_thread.get_id());
  thread_state.assigned_thread_ids = true;
  
  //  Main thread event loop
  eowu::thread::events(thread_state, context_manager);
  
  //  When the events thread is finished (e.g., when the escape key is pressed),
  //  attempt to wait for the render and task threads to finish. If they don't complete
  //  within N seconds, throw an error.
  
  bool threads_completed = eowu::thread::try_await_thread_finish(thread_state, std::chrono::seconds(10));
  
  if (threads_completed) {
    render_thread.join();
    task_thread.join();
  }
  
  return 0;
}
