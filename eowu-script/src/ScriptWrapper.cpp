//
//  ScriptWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#include "ScriptWrapper.hpp"
#include "StateWrapper.hpp"
#include "ModelWrapper.hpp"
#include "KeyboardWrapper.hpp"
#include "RendererWrapper.hpp"
#include "VariableWrapper.hpp"
#include "Constants.hpp"
#include "LockedLuaRenderFunctions.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "RuntimeUtil.hpp"
#include "parser/ParseUtil.hpp"
#include <eowu-common/config.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-data.hpp>
#include <assert.h>

//  states
eowu::StateWrapperContainerType eowu::ScriptWrapper::states = nullptr;
//
//  gl pipeline
std::shared_ptr<eowu::GLPipeline> eowu::ScriptWrapper::pipeline = nullptr;
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
//  target sets
std::unordered_map<std::string, std::unique_ptr<eowu::TargetSetWrapper>> eowu::ScriptWrapper::target_sets{};
//
//  timeouts
eowu::TimeoutWrapperContainerType eowu::ScriptWrapper::timeout_wrappers = nullptr;
//
//  keyboard
std::unique_ptr<eowu::KeyboardWrapper> eowu::ScriptWrapper::keyboard = nullptr;
//
//  lua task context
std::shared_ptr<eowu::LuaContext> eowu::ScriptWrapper::lua_task_context = nullptr;
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
  eowu::ScriptWrapper::lua_task_context = context;
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

void eowu::ScriptWrapper::SetTimeoutWrapperContainer(eowu::TimeoutWrapperContainerType timeouts) {
  eowu::ScriptWrapper::timeout_wrappers = timeouts;
}

void eowu::ScriptWrapper::SetXYTargets(const std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> &targets) {
  eowu::ScriptWrapper::xy_targets = targets;
}

int eowu::ScriptWrapper::SetRenderFunctionPair(lua_State *L) {
  const char* const func_id = "Render";
  
  int n_inputs = lua_gettop(L);
  
  eowu::LuaFunction *render_func = nullptr;
  eowu::LuaFunction *flip_func = nullptr;
  
  if (n_inputs == 1) {
    const auto msg = eowu::util::get_message_not_enough_inputs(func_id, 1, 0);
    throw eowu::LuaError(msg);
  }
  
  int input_index = n_inputs > 2 ? -2 : -1;
  render_func = get_function_from_state(L, input_index, render_functions.get(), "render");
  
  if (n_inputs > 2) {
    flip_func = get_function_from_state(L, -1, flip_functions.get(), "flip");
  }
  
  //  If this is the render thread, don't attempt
  //  to queue the functions -- just set them on the next frame.
  if (is_render_thread()) {
    lua_render_thread_functions->Set(render_func, flip_func);
  } else {
    lua_render_thread_functions->Queue(render_func, flip_func);
  }
  
  return 0;
}

double eowu::ScriptWrapper::GetEllapsedTime() const {
  return eowu::ScriptWrapper::state_runner->GetTimer().Ellapsed().count();
}

eowu::ModelWrapper eowu::ScriptWrapper::GetModelWrapper(const std::string &id) const {
  assert(IsComplete());
    
  auto model = pipeline->GetResourceManager()->Get<eowu::Model>(id);
  auto renderer = pipeline->GetRenderer();
  auto texture_manager = pipeline->GetTextureManager();
  auto window_container = pipeline->GetWindowContainer();
  
  eowu::ModelWrapper model_wrapper(model, renderer, window_container, texture_manager);
  
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

eowu::TimeoutWrapper* eowu::ScriptWrapper::GetTimeoutWrapper(const std::string &id) {
  assert(IsComplete());
  
  eowu::TimeoutWrapper *wrapper = nullptr;
  
  timeout_wrappers->Use([&](auto *target_map) -> void {
    const auto &it = target_map->find(id);
    
    if (it == target_map->end()) {
      throw eowu::NonexistentResourceError::MessageKindId("Timeout", id);
    }
    
    wrapper = it->second.get();
  });
  
  return wrapper;
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

eowu::RendererWrapper eowu::ScriptWrapper::GetRendererWrapper() const {
  assert(IsComplete());
  
  auto renderer = pipeline->GetRenderer();
  eowu::RendererWrapper render_wrapper(renderer);
  
  return render_wrapper;
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

eowu::TimeoutWrapper* eowu::ScriptWrapper::MakeTimeout(const std::string &id, int ms, luabridge::LuaRef func) {
  const char* const func_id = "MakeTimeout";
  
  //  Ensure we're calling from the task thread.
  if (!is_task_thread()) {
    throw eowu::LuaError(eowu::util::get_message_wrong_thread(func_id, "Render"));
  }
  
  if (!func.isFunction()) {
    lua_State *L = func.state();
    auto type = func.type();
    
    std::string msg = eowu::util::get_message_wrong_input_type(func_id, "function", lua_typename(L, type));
    
    throw eowu::LuaError(msg);
  }
  
  eowu::TimeoutWrapper *wrapper = nullptr;
  eowu::time::DurationType duration = std::chrono::milliseconds(ms);
  
  eowu::ScriptWrapper::timeout_wrappers->Use([&](auto &timeout_map) -> void {
    auto wrapper_to_store = std::make_unique<eowu::TimeoutWrapper>(lua_task_context, func, duration);
    wrapper = wrapper_to_store.get();
    
    timeout_map[id] = std::move(wrapper_to_store);
  });
  
  assert(wrapper);
  
  return wrapper;
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
  
  auto target_set = std::make_unique<eowu::TargetSetWrapper>(lua_task_context, target_ptrs);
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
  using eowu::util::get_message_wrong_input_type;
  
  auto ref = luabridge::LuaRef::fromStack(L, stack_index);
  
  if (!ref.isString()) {
    const std::string msg = get_message_wrong_input_type("Render", "function", lua_typename(L, ref.type()));
    throw eowu::LuaError(msg);
  }
  
  const std::string func_id = ref.cast<std::string>();
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

void eowu::ScriptWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::ScriptWrapper>(eowu::constants::eowu_script_name)
  .addConstructor<void(*)(void)>()
  .addFunction("Commit", &eowu::ScriptWrapper::CommitData)
  .addFunction("Stimulus", &eowu::ScriptWrapper::GetModelWrapper)
  .addFunction("Target", &eowu::ScriptWrapper::GetTargetWrapper)
  .addFunction("Timeout", &eowu::ScriptWrapper::GetTimeoutWrapper)
  .addFunction("State", &eowu::ScriptWrapper::GetStateWrapper)
  .addFunction("Render", &eowu::ScriptWrapper::SetRenderFunctionPair)
  .addFunction("Renderer", &eowu::ScriptWrapper::GetRendererWrapper)
  .addFunction("Variable", &eowu::ScriptWrapper::GetVariable)
  .addFunction("Keyboard", &eowu::ScriptWrapper::GetKeyboardWrapper)
  .addFunction("Ellapsed", &eowu::ScriptWrapper::GetEllapsedTime)
  .addFunction("Exit", &eowu::ScriptWrapper::Exit)
  .addFunction("MakeTargetSet", &eowu::ScriptWrapper::MakeTargetSet)
  .addFunction("MakeTimeout", &eowu::ScriptWrapper::MakeTimeout)
  .endClass()
  .endNamespace();
}
