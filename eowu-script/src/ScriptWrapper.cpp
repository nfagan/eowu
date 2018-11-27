//
//  ScriptWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#include "ScriptWrapper.hpp"
#include "Wrappers.hpp"
#include "Constants.hpp"
#include "LockedLuaRenderFunctions.hpp"
#include "TimeoutHandle.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "RuntimeUtil.hpp"
#include "parser/ParseUtil.hpp"
#include <eowu-common/config.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-audio.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-data.hpp>
#include <assert.h>

//  states
eowu::StateWrapperContainerType eowu::ScriptWrapper::states = nullptr;
//
//  gl pipeline
std::shared_ptr<eowu::GLPipeline> eowu::ScriptWrapper::pipeline = nullptr;
//
//  audio context
std::shared_ptr<eowu::AudioContext> eowu::ScriptWrapper::audio_context = nullptr;
//
//  render functions
eowu::LuaFunctionContainerType eowu::ScriptWrapper::render_functions = nullptr;
//
//  flip functions
eowu::LuaFunctionContainerType eowu::ScriptWrapper::flip_functions = nullptr;
//
//  render-flip pair
std::shared_ptr<eowu::LockedLuaRenderFunctions> eowu::ScriptWrapper::lua_render_thread_functions = nullptr;
//
//  task data store
std::shared_ptr<eowu::data::Store> eowu::ScriptWrapper::task_data_store = nullptr;
//
//  variables
eowu::ScriptWrapper::Variables eowu::ScriptWrapper::variables{};
//
//  target-wrappers
std::unordered_map<std::string, std::shared_ptr<eowu::TargetWrapper>> eowu::ScriptWrapper::target_wrappers{};
//
//  xy-targets
std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> eowu::ScriptWrapper::xy_targets{};
//
//  sounds
std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>> eowu::ScriptWrapper::sounds{};
//
//  target sets
std::unordered_map<std::string, std::unique_ptr<eowu::TargetSetWrapper>> eowu::ScriptWrapper::target_sets{};
//
//  timeouts
eowu::TimeoutAggregateMapType eowu::ScriptWrapper::timeout_wrappers{};
//
//  intervals
eowu::TimeoutAggregateMapType eowu::ScriptWrapper::interval_wrappers{};
//
//  keyboard
std::unique_ptr<eowu::KeyboardWrapper> eowu::ScriptWrapper::keyboard = nullptr;
//
//  lua task context
eowu::ScriptWrapper::LuaContexts eowu::ScriptWrapper::lua_contexts{};
//
//  thread ids
eowu::ScriptWrapper::ThreadIds eowu::ScriptWrapper::thread_ids{};
//
//  state runner
eowu::StateRunner *eowu::ScriptWrapper::state_runner = nullptr;

bool eowu::ScriptWrapper::IsComplete() const {
#ifdef EOWU_DEBUG
  bool sc = states != nullptr;
  bool pc = pipeline != nullptr;
  bool rc = render_functions != nullptr;
  bool lc = lua_render_thread_functions != nullptr;
  bool kc = keyboard != nullptr;
  bool src = state_runner != nullptr;
  
  return sc && pc && rc && lc && kc && src;
#else
  return true;
#endif
}

void eowu::ScriptWrapper::SetLuaTaskContext(std::shared_ptr<eowu::LuaContext> context) {
  eowu::ScriptWrapper::lua_contexts.task = context;
}

void eowu::ScriptWrapper::SetLuaRenderContext(std::shared_ptr<eowu::LuaContext> context) {
  eowu::ScriptWrapper::lua_contexts.render = context;
}

void eowu::ScriptWrapper::SetThreadIds(const std::thread::id &render, const std::thread::id &task) {
  eowu::ScriptWrapper::thread_ids.render = render;
  eowu::ScriptWrapper::thread_ids.task = task;
}

void eowu::ScriptWrapper::SetTaskDataStore(std::shared_ptr<eowu::data::Store> store) {
  task_data_store = store;
}

void eowu::ScriptWrapper::SetStateRunner(eowu::StateRunner *runner) {
  eowu::ScriptWrapper::state_runner = runner;
}

void eowu::ScriptWrapper::SetKeyboardWrapper(std::unique_ptr<eowu::KeyboardWrapper> keyboard) {
  eowu::ScriptWrapper::keyboard = std::move(keyboard);
}

void eowu::ScriptWrapper::SetLockedRenderFunctions(std::shared_ptr<eowu::LockedLuaRenderFunctions> locked_functions) {
  lua_render_thread_functions = locked_functions;
}

void eowu::ScriptWrapper::SetVariables(const std::unordered_map<std::string, eowu::data::Commitable> &variables) {  
  eowu::ScriptWrapper::variables.active = variables;
  eowu::ScriptWrapper::variables.defaults = variables;
}

void eowu::ScriptWrapper::SetStateWrapperContainer(eowu::StateWrapperContainerType states) {
  eowu::ScriptWrapper::states = std::move(states);
}

void eowu::ScriptWrapper::SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline) {
  eowu::ScriptWrapper::pipeline = pipeline;
}

void eowu::ScriptWrapper::SetAudioContext(std::shared_ptr<eowu::AudioContext> context) {
  eowu::ScriptWrapper::audio_context = context;
}

void eowu::ScriptWrapper::SetSounds(const std::unordered_map<std::string,
                                    std::shared_ptr<eowu::AudioBufferSource>> &sources) {
  eowu::ScriptWrapper::sounds = sources;
}

void eowu::ScriptWrapper::SetLuaRenderFunctionPair(std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions) {
  lua_render_thread_functions = lua_render_functions;
}

void eowu::ScriptWrapper::SetRenderFunctions(eowu::LuaFunctionContainerType render_functions) {
  eowu::ScriptWrapper::render_functions = std::move(render_functions);
}

void eowu::ScriptWrapper::SetFlipFunctions(eowu::LuaFunctionContainerType flip_functions) {
  eowu::ScriptWrapper::flip_functions = std::move(flip_functions);
}

void eowu::ScriptWrapper::SetTargetWrapperContainer(const std::unordered_map<std::string, std::shared_ptr<eowu::TargetWrapper>> &targets) {
  eowu::ScriptWrapper::target_wrappers = targets;
}

void eowu::ScriptWrapper::SetXYTargets(const std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> &targets) {
  eowu::ScriptWrapper::xy_targets = targets;
}

int eowu::ScriptWrapper::SetRenderFunctionPair(lua_State *L) {
  const char* const func_id = "Render";
  
  int n_inputs = lua_gettop(L);
  
  eowu::LuaFunction *render_func = nullptr;
  eowu::LuaFunction *flip_func = nullptr;
  
  int render_stack_index = -3;
  int flip_stack_index = -2;
  int offset = 0;
  
  bool is_async = true;
  
  if (n_inputs == 1) {
    throw eowu::LuaError(util::get_message_not_enough_inputs(func_id, 1, 0));
  } else if (n_inputs > 4) {
    throw eowu::LuaError(util::get_message_wrong_number_of_inputs(func_id, 3, n_inputs));
  }
  
  if (n_inputs == 2) {
    offset = 2;
  } else if (n_inputs == 3) {
    offset = 1;
  } else {
    offset = 0;
    is_async = eowu::parser::get_bool_or_error_from_state(L, -1);
  }
  
  render_stack_index += offset;
  flip_stack_index += offset;
  
  if (!lua_isnil(L, render_stack_index)) {
    render_func = get_function_from_state(L, render_stack_index, render_functions.get(), "render");
  }
  
  if (n_inputs > 2 && !lua_isnil(L, flip_stack_index)) {
    flip_func = get_function_from_state(L, flip_stack_index, flip_functions.get(), "flip");
  }
  
  //  If this is the render thread, don't attempt
  //  to queue the functions -- just set them on the next frame.
  if (is_render_thread() || !is_async) {
    lua_render_thread_functions->Set(render_func, flip_func);
  } else {
    lua_render_thread_functions->Queue(render_func, flip_func);
  }
  
  return 0;
}

double eowu::ScriptWrapper::GetEllapsedTime() const {
  return eowu::ScriptWrapper::state_runner->GetTimer().Ellapsed().count();
}

const eowu::TimeoutAggregateMapType* eowu::ScriptWrapper::GetIntervalWrappers() const {
  return &eowu::ScriptWrapper::interval_wrappers;
}

const eowu::TimeoutAggregateMapType* eowu::ScriptWrapper::GetTimeoutWrappers() const {
  return &eowu::ScriptWrapper::timeout_wrappers;
}

eowu::ModelWrapper eowu::ScriptWrapper::GetModelWrapper(const std::string &id) const {
  assert(IsComplete());
  
  auto resource_manager = pipeline->GetResourceManager();
  auto model = resource_manager->Get<eowu::Model>(id);
  auto renderer = pipeline->GetRenderer();
  auto texture_manager = pipeline->GetTextureManager();
  auto window_container = pipeline->GetWindowContainer();
  
  eowu::ModelWrapper model_wrapper(model,
                                   resource_manager,
                                   renderer,
                                   window_container,
                                   texture_manager);
  
  return model_wrapper;
}

eowu::TargetWrapper* eowu::ScriptWrapper::GetTargetWrapper(const std::string &id) {
  assert(IsComplete());
  
  const auto &it = target_wrappers.find(id);
  
  if (it == target_wrappers.end()) {
    throw eowu::NonexistentResourceError::MessageKindId("Target", id);
  }
  
  return it->second.get();
}

eowu::TargetSetWrapper* eowu::ScriptWrapper::GetTargetSetWrapper(const std::string &id) {
  assert(IsComplete());
  
  const auto &it = target_sets.find(id);
  
  if (it == target_sets.end()) {
    throw eowu::NonexistentResourceError::MessageKindId("TargetSet", id);
  }
  
  return it->second.get();
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::GetTimeoutHandleWrapper(const std::string &id) {
  return get_timeout_handle_wrapper(id, &timeout_wrappers, "Timeout");
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::GetIntervalHandleWrapper(const std::string &id) {
  return get_timeout_handle_wrapper(id, &interval_wrappers, "Interval");
}

eowu::KeyboardWrapper* eowu::ScriptWrapper::GetKeyboardWrapper() const {
  assert(IsComplete());
  
  return keyboard.get();
}

eowu::StateWrapper* eowu::ScriptWrapper::GetStateWrapper(const std::string &id) const {
  assert(IsComplete());
  
  const auto &it = states->find(id);
  
  if (it == states->end()) {
    throw eowu::NonexistentResourceError::MessageKindId("State", id);
  }
  
  return it->second.get();
}

eowu::StateWrapper* eowu::ScriptWrapper::GetActiveStateWrapper() const {
  assert(IsComplete());
  
  const eowu::State *active_state = state_runner->GetActiveState();
  
  if (active_state == nullptr) {
    throw eowu::LuaError("Requested a handle to the active state, but no active state was present.");
  }
  
  return GetStateWrapper(active_state->GetId());
}

eowu::RendererWrapper eowu::ScriptWrapper::GetRendererWrapper() const {
  assert(IsComplete());
  
  auto renderer = pipeline->GetRenderer();
  eowu::RendererWrapper render_wrapper(renderer);
  
  return render_wrapper;
}

eowu::WindowWrapper eowu::ScriptWrapper::GetWindowWrapper(const std::string &id) {
  assert(IsComplete());
  
  auto window_container = pipeline->GetWindowContainer();
  
  if (!window_container->Has(id)) {
    throw eowu::NonexistentResourceError::MessageKindId("Window", id);
  }
  
  eowu::WindowWrapper window(window_container->Get(id));
  
  return window;
}

eowu::VariableWrapper eowu::ScriptWrapper::GetVariable(const std::string &id) {  
  auto it = variables.active.find(id);
  
  if (it == variables.active.end()) {
    throw eowu::NonexistentResourceError::MessageKindId("Variable", id);
  }
  
  eowu::data::Commitable *active = &it->second;
  eowu::data::Commitable *defaults = &variables.defaults.at(id);
  
  eowu::VariableWrapper wrapper(active, defaults);
  
  return wrapper;
}

eowu::AudioSourceWrapper eowu::ScriptWrapper::GetAudioSourceWrapper(const std::string &id) const {
  auto it = sounds.find(id);
  
  if (it == sounds.end()) {
    throw eowu::NonexistentResourceError::MessageKindId("Sound", id);
  }
  
  auto source_aggregate = audio_context->GetSourceAggregate();
  auto buffer_source = it->second;
  
  eowu::AudioSourceWrapper source_wrapper(source_aggregate, buffer_source);
  
  return source_wrapper;
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::MakeTimeout(const std::string &id, int ms, luabridge::LuaRef func) {
  auto *wrappers = &eowu::ScriptWrapper::timeout_wrappers;
  return make_timeout(wrappers, id, ms, func, "MakeTimeout", eowu::Timeout::TIMEOUT);
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::MakeInterval(const std::string &id, int ms, luabridge::LuaRef func) {
  auto *wrappers = &eowu::ScriptWrapper::interval_wrappers;
  return make_timeout(wrappers, id, ms, func, "MakeInterval", eowu::Timeout::INTERVAL);
}

eowu::TargetSetWrapper* eowu::ScriptWrapper::MakeTargetSet(const std::string &id, lua_State *L) {
  const char* const func_id = "MakeTargetSet";
  
  //  Ensure we're calling from the task thread.
  if (!is_task_thread()) {
    throw eowu::LuaError(eowu::util::get_message_wrong_thread(func_id, "Render"));
  }
  
  int n_inputs = lua_gettop(L);
  
  if (n_inputs != 3) {
    throw eowu::LuaError(eowu::util::get_message_wrong_number_of_inputs(func_id, 2, n_inputs-1));
  }
  
  std::vector<std::string> target_ids;
  
  try {
    target_ids = eowu::parser::get_string_vector_from_state(L, -1);
  } catch (const std::exception &e) {
    int type = lua_type(L, -1);
    std::string msg = eowu::util::get_message_wrong_input_type(func_id, "string-array", lua_typename(L, type));
    throw eowu::LuaError(msg);
  }

  std::vector<eowu::XYTarget*> target_ptrs;

  for (const auto &targ_id : target_ids) {
    const auto &it = xy_targets.find(targ_id);
    
    if (it == xy_targets.end()) {
      auto msg = eowu::util::get_message_nonexistent_resource(func_id, "Target", targ_id);
      throw eowu::NonexistentResourceError(msg);
    }
    
    target_ptrs.push_back(it->second.get());
  }
  
  auto target_set = std::make_unique<eowu::TargetSetWrapper>(lua_contexts.task, target_ptrs);
  eowu::TargetSetWrapper *ptr = target_set.get();
  target_sets[id] = std::move(target_set);
  
  return ptr;
}

std::shared_ptr<eowu::LockedLuaRenderFunctions> eowu::ScriptWrapper::GetLockedRenderFunctions() const {
  return eowu::ScriptWrapper::lua_render_thread_functions;
}

void eowu::ScriptWrapper::Exit() {
  assert(state_runner);
  
  state_runner->Exit();
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::get_timeout_handle_wrapper(const std::string &id,
                                                                           const eowu::TimeoutAggregateMapType *aggregate,
                                                                           const char* const kind) {
  eowu::TimeoutHandleWrapper timeout_wrapper;
  
  aggregate->Use([&](const auto &map) -> void {
    const auto &it = map.find(id);
    
    if (it == map.end()) {
      throw eowu::NonexistentResourceError::MessageKindId(kind, id);
    }
    
    timeout_wrapper = it->second.handle;
  });
  
  return timeout_wrapper;
}

eowu::TimeoutHandleWrapper eowu::ScriptWrapper::make_timeout(eowu::TimeoutAggregateMapType *aggregate,
                                                             const std::string &id,
                                                             int ms,
                                                             const luabridge::LuaRef &func,
                                                             const char* const make_func_id,
                                                             eowu::Timeout::Type timeout_type) {
  //  Ensure we're calling from the task thread.
  if (!is_task_thread()) {
    throw eowu::LuaError(eowu::util::get_message_wrong_thread(make_func_id, "Render"));
  }
  
  auto lua_context = lua_contexts.task;
  
  if (!func.isFunction()) {
    lua_State *L = func.state();
    auto type = func.type();
    
    std::string msg = eowu::util::get_message_wrong_input_type(make_func_id, "function", lua_typename(L, type));
    
    throw eowu::LuaError(msg);
  }
  
  eowu::time::DurationType duration = std::chrono::milliseconds(ms);
  
  eowu::TimeoutHandleWrapper handle_wrapper;
  
  aggregate->Use([&](auto &map) -> void {
    auto timeout_wrapper = std::make_shared<eowu::TimeoutWrapper>(lua_context, func, timeout_type, duration);
    auto timeout_handle = std::make_shared<eowu::TimeoutHandle>(timeout_wrapper.get());
    
    auto it = map.find(id);
    
    //  We already have a handle / wrapper with this id.
    if (it != map.end()) {
      auto &aggregate = it->second;
      
      aggregate.wrapper->Cancel();
      aggregate.handle->Invalidate();
      aggregate.handle = timeout_handle;
      
      aggregate.wrapper = timeout_wrapper;
    } else {
      eowu::TimeoutAggregateContents timeout_aggregate{timeout_wrapper, timeout_handle};
      
      map.emplace(id, timeout_aggregate);
    }
    
    handle_wrapper = eowu::TimeoutHandleWrapper(timeout_handle);
  });
  
  return handle_wrapper;
}

void eowu::ScriptWrapper::CommitData() const {
  std::vector<char> into;
  
  eowu::serialize::unsafe_nest_aggregate("Data", 2, into);
  
  //  serialize task variables
  commit_variables(into);
  
  //  serialize states
  commit_states(into);
  
  task_data_store->Write(into);
}

void eowu::ScriptWrapper::commit_states(std::vector<char> &into) const {
  //  Serialize each state's variables + timing data.
  
  const auto &state_container = *states.get();
  const auto n_states = state_container.size();
  
  eowu::serialize::unsafe_nest_aggregate(eowu::constants::eowu_states_table_name, n_states, into);
  
  for (const auto &it : state_container) {
    const auto &state = it.second;
    
    state->Write(into);
  }
}

void eowu::ScriptWrapper::commit_variables(std::vector<char> &into) const {
  //  Serialize each committed variable.
  
  const auto &variables = eowu::ScriptWrapper::variables.active;
  
  std::size_t n_committed = 0;
  
  for (const auto &it : variables) {
    if (it.second.IsCommitted()) {
      n_committed++;
    }
  }
  
  eowu::serialize::unsafe_nest_aggregate(eowu::constants::eowu_variables_name, n_committed, into);
  
  for (const auto &it : variables) {
    const auto &variable = it.second;
    
    if (!variable.IsCommitted()) {
      continue;
    }
    
    const auto inserter = [&](const auto &data) -> void {
      eowu::serialize::serialize(data, into);
    };
    
    variable.Use(inserter);
  }
}

eowu::LuaFunction* eowu::ScriptWrapper::get_function_from_state(lua_State *L,
                                                                int stack_index,
                                                                eowu::LuaFunctionMapType *funcs,
                                                                const std::string &kind) {
  auto func_id = eowu::parser::get_string_or_error_from_state(L, stack_index);
  const auto &it = funcs->find(func_id);
  
  if (it == funcs->end()) {
    throw eowu::NonexistentResourceError::MessageKindId(kind, func_id);
  }
  
  return &it->second;
}

bool eowu::ScriptWrapper::is_render_thread() {
  return std::this_thread::get_id() == eowu::ScriptWrapper::thread_ids.render;
}

bool eowu::ScriptWrapper::is_task_thread() {
  return std::this_thread::get_id() == eowu::ScriptWrapper::thread_ids.task;
}

std::shared_ptr<eowu::LuaContext> eowu::ScriptWrapper::get_lua_context_for_thread() {
  if (is_task_thread()) {
    return lua_contexts.task;
  } else if (is_render_thread()) {
    return lua_contexts.render;
  } else {
    throw eowu::LuaError("Unrecognized thread type.");
  }
}

void eowu::ScriptWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::ScriptWrapper>(eowu::constants::eowu_script_name)
  .addConstructor<void(*)(void)>()
  .addFunction("Commit", &eowu::ScriptWrapper::CommitData)
  .addFunction("Stimulus", &eowu::ScriptWrapper::GetModelWrapper)
  .addFunction("Target", &eowu::ScriptWrapper::GetTargetWrapper)
  .addFunction("TargetSet", &eowu::ScriptWrapper::GetTargetSetWrapper)
  .addFunction("Timeout", &eowu::ScriptWrapper::GetTimeoutHandleWrapper)
  .addFunction("Interval", &eowu::ScriptWrapper::GetIntervalHandleWrapper)
  .addFunction("State", &eowu::ScriptWrapper::GetStateWrapper)
  .addFunction("CurrentState", &eowu::ScriptWrapper::GetActiveStateWrapper)
  .addFunction("Render", &eowu::ScriptWrapper::SetRenderFunctionPair)
  .addFunction("Renderer", &eowu::ScriptWrapper::GetRendererWrapper)
  .addFunction("Variable", &eowu::ScriptWrapper::GetVariable)
  .addFunction("Window", &eowu::ScriptWrapper::GetWindowWrapper)
  .addFunction("Keyboard", &eowu::ScriptWrapper::GetKeyboardWrapper)
  .addFunction("Sound", &eowu::ScriptWrapper::GetAudioSourceWrapper)
  .addFunction("Ellapsed", &eowu::ScriptWrapper::GetEllapsedTime)
  .addFunction("Exit", &eowu::ScriptWrapper::Exit)
  .addFunction("MakeTargetSet", &eowu::ScriptWrapper::MakeTargetSet)
  .addFunction("MakeTimeout", &eowu::ScriptWrapper::MakeTimeout)
  .addFunction("MakeInterval", &eowu::ScriptWrapper::MakeInterval)
  .endClass()
  .endNamespace();
}
