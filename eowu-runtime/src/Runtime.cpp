//
//  Runtime.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Runtime.hpp"
#include "GLInit.hpp"
#include "AudioInit.hpp"
#include "DataInit.hpp"
#include "SourceInit.hpp"
#include "TargetInit.hpp"
#include "LuaInit.hpp"
#include "Threads.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-audio.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-common/Timer.hpp>
#include <Lua.hpp>
#include <iostream>
#include <thread>

int eowu::runtime::main(const std::string &file) {
  eowu::StateManager state_manager;
  eowu::StateRunner state_runner;
  
  auto lua_context_result = eowu::init::initialize_lua_contexts();
  auto setup_schema_result = eowu::init::initialize_schema(file, lua_context_result.result.task);
  
  if (!setup_schema_result.status.success) {
    setup_schema_result.status.file = file;
    setup_schema_result.status.print();
    return EXIT_FAILURE;
  }
  
  const auto &setup_schema = setup_schema_result.result;
  const auto &lua_contexts = lua_context_result.result;
  
  //  task data store
  const auto &data_root_directory = setup_schema.paths.data;
  const auto &save_schema = setup_schema.save;
  
  auto data_init_result = eowu::data::initialize_data_pipeline(data_root_directory, save_schema);
  
  if (!data_init_result.status.success) {
    data_init_result.status.file = file;
    data_init_result.status.print();
    return EXIT_FAILURE;
  }
  
  eowu::ScriptWrapper::SetTaskDataStore(data_init_result.result.task_data_store);
  
  //  sounds
  auto sound_status = eowu::init::initialize_audio_pipeline(setup_schema.sounds);
  
  if (!sound_status.status.success) {
    sound_status.status.file = file;
    sound_status.status.print();
    return EXIT_FAILURE;
  }
  
  auto audio_context = sound_status.result.audio_context;
  
  eowu::ScriptWrapper::SetAudioContext(audio_context);
  eowu::ScriptWrapper::SetSounds(sound_status.result.sounds);
  
  //  gl pipeline
  auto gl_pipeline = eowu::GLPipeline::GetInstance();
  auto gl_init = init::initialize_gl_pipeline(gl_pipeline, setup_schema);
  
  if (!gl_init.status.success) {
    gl_init.status.file = file;
    gl_init.status.print();
    return EXIT_FAILURE;
  }
  
  eowu::ScriptWrapper::SetTextModels(gl_init.result.text_models);
  
  //  sources
  auto source_status = eowu::init::initialize_sources(setup_schema.sources, gl_pipeline);
  
  if (!source_status.status.success) {
    source_status.status.file = file;
    source_status.status.print();
    return EXIT_FAILURE;
  }
  
  const auto &xy_source_init = source_status.result.xy_source_init;
  const auto &xy_sources = xy_source_init.xy_sources;
  const auto &xy_source_window_mapping = xy_source_init.xy_source_window_mapping;
  
  //  targets
  auto target_status = eowu::init::initialize_targets(setup_schema.targets,
                                                      xy_sources,
                                                      xy_source_window_mapping,
                                                      lua_contexts.task,
                                                      gl_pipeline);
  
  if (!target_status.status.success) {
    target_status.status.file = file;
    target_status.status.print();
    return EXIT_FAILURE;
  }
  
  //  maps each target to an id.
  const auto &target_map = target_status.result.targets;
  const auto &target_wrappers = target_status.result.target_wrappers;
  
  eowu::ScriptWrapper::SetTargetWrapperContainer(target_wrappers);
  eowu::ScriptWrapper::SetXYTargets(target_map);
  eowu::ScriptWrapper::SetXYSources(xy_sources);
  
  //
  //  make timeout wrappers
  const auto *interval_wrappers = eowu::ScriptWrapper::GetIntervalWrappers();
  const auto *timeout_wrappers = eowu::ScriptWrapper::GetTimeoutWrappers();
  
  //
  //  otherwise, the gl pipeline and sources are ok.
  auto script_wrapper_result = eowu::init::initialize_script_wrapper(file,
                                                                     setup_schema,
                                                                     gl_pipeline,
                                                                     lua_contexts,
                                                                     state_manager,
                                                                     state_runner);
  
  if (!script_wrapper_result.status.success) {
    script_wrapper_result.status.file = file;
    script_wrapper_result.status.print();
    return EXIT_FAILURE;
  }
  
  //
  //  Shared state
  eowu::thread::SharedState thread_state(&state_runner.GetTimer());
  
  //
  //  Task thread
  auto vec_targets = eowu::validate::get_values(target_map);
  
  state_runner.Begin(script_wrapper_result.result.first_state);
  
  auto task_thread = std::thread(eowu::thread::task,
                                 std::ref(thread_state),
                                 std::ref(state_runner),
                                 std::ref(vec_targets),
                                 timeout_wrappers,
                                 interval_wrappers);

  //
	//	Detach gl-context from main thread.
	auto context_manager = gl_pipeline->GetContextManager();
	context_manager->DetachCurrentContext();
  
  //
  //  Render thread
  auto locked_lua_functions = eowu::ScriptWrapper::GetLockedRenderFunctions();
  const auto &lua_render_context = lua_contexts.render;
  
  auto render_thread = std::thread(eowu::thread::render,
                                   std::ref(thread_state),
                                   lua_render_context,
                                   locked_lua_functions,
                                   gl_pipeline);
  
  //
  //  Assign thread ids
  eowu::ScriptWrapper::SetThreadIds(render_thread.get_id(), task_thread.get_id());
  thread_state.assigned_thread_ids = true;
  
  //
  //  Main thread event loop
  int stop_key_code = eowu::Keyboard::GetKeyCode("escape");
  eowu::thread::events(thread_state, context_manager, audio_context, stop_key_code);
  
  //  When the events thread is finished (e.g., when the escape key is pressed),
  //  attempt to wait for the render and task threads to finish. If they don't complete
  //  within N seconds, throw an error.
  
  bool threads_completed = eowu::thread::try_await_thread_finish(thread_state, std::chrono::seconds(10));
  
  if (threads_completed) {
    render_thread.join();
    task_thread.join();
  }
  
  return EXIT_SUCCESS;
}
