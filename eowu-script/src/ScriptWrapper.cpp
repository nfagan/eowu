//
//  ScriptWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#include "ScriptWrapper.hpp"
#include "StateWrapper.hpp"
#include "ModelWrapper.hpp"
#include "RendererWrapper.hpp"
#include "VariableWrapper.hpp"
#include "Constants.hpp"
#include "LockedLuaRenderFunctions.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include <eowu-common/config.hpp>
#include <eowu-gl/eowu-gl.hpp>
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

bool eowu::ScriptWrapper::IsComplete() const {
#ifdef EOWU_DEBUG
  bool sc = states != nullptr;
  bool pc = pipeline != nullptr;
  bool rc = render_functions != nullptr;
  bool lc = lua_render_thread_functions != nullptr;
  
  return sc && pc && rc && lc;
#else
  return true;
#endif
}

void eowu::ScriptWrapper::SetTaskDataStore(std::shared_ptr<eowu::data::Store> store) {
  task_data_store = store;
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

void eowu::ScriptWrapper::SetRenderFunctionPair(const std::string &render_id, const std::string &flip_id) {  
  const auto &render_it = render_functions->find(render_id);
  const auto &flip_it = flip_functions->find(flip_id);
  
  if (render_it == render_functions->end()) {
    throw eowu::NonexistentResourceError::MessageKindId("Render function", render_id);
  }
  
  if (flip_it == flip_functions->end()) {
    throw eowu::NonexistentResourceError::MessageKindId("Flip function", flip_id);
  }
  
  eowu::LuaFunction *render_func = &render_it->second;
  eowu::LuaFunction *flip_func = &flip_it->second;
  
  lua_render_thread_functions->Queue(render_func, flip_func);
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

std::shared_ptr<eowu::LockedLuaRenderFunctions> eowu::ScriptWrapper::GetLockedRenderFunctions() const {
  return eowu::ScriptWrapper::lua_render_thread_functions;
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

void eowu::ScriptWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginClass<eowu::ScriptWrapper>(eowu::constants::eowu_script_name)
  .addConstructor<void(*)(void)>()
  .addFunction("Commit", &eowu::ScriptWrapper::CommitData)
  .addFunction("Stimulus", &eowu::ScriptWrapper::GetModelWrapper)
  .addFunction("State", &eowu::ScriptWrapper::GetStateWrapper)
  .addFunction("Render", &eowu::ScriptWrapper::SetRenderFunctionPair)
  .addFunction("Renderer", &eowu::ScriptWrapper::GetRendererWrapper)
  .addFunction("Variable", &eowu::ScriptWrapper::GetVariable)
  .endClass();
}
