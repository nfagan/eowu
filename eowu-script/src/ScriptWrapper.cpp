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
#include "Constants.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include <eowu-common/config.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-data.hpp>
#include <assert.h>

//  states
eowu::StateWrapperContainerType eowu::ScriptWrapper::states = nullptr;
//  gl pipeline
std::shared_ptr<eowu::GLPipeline> eowu::ScriptWrapper::pipeline = nullptr;
//  render functions
eowu::LuaFunctionContainerType eowu::ScriptWrapper::render_functions = nullptr;
//  flip functions
eowu::LuaFunctionContainerType eowu::ScriptWrapper::flip_functions = nullptr;
//  render function
std::shared_ptr<eowu::LuaFunction> eowu::ScriptWrapper::LuaRenderFunction = nullptr;
//  flip function
std::shared_ptr<eowu::LuaFunction> eowu::ScriptWrapper::LuaFlipFunction = nullptr;
//  task data store
std::shared_ptr<eowu::data::Store> eowu::ScriptWrapper::task_data_store = nullptr;

bool eowu::ScriptWrapper::IsComplete() const {
#ifdef EOWU_DEBUG
  bool sc = states != nullptr;
  bool pc = pipeline != nullptr;
  bool rc = render_functions != nullptr;
  bool lc = LuaRenderFunction != nullptr;
  bool fc = LuaFlipFunction != nullptr;
  
  return sc && pc && rc && lc && fc;
#else
  return true;
#endif
}

void eowu::ScriptWrapper::SetStateWrapperContainer(eowu::StateWrapperContainerType states) {
  eowu::ScriptWrapper::states = std::move(states);
}

void eowu::ScriptWrapper::SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline) {
  eowu::ScriptWrapper::pipeline = pipeline;
}

void eowu::ScriptWrapper::SetLuaRenderFunction(std::shared_ptr<eowu::LuaFunction> lua_render_function) {
  eowu::ScriptWrapper::LuaRenderFunction = lua_render_function;
}

void eowu::ScriptWrapper::SetLuaFlipFunction(std::shared_ptr<eowu::LuaFunction> lua_flip_function) {
  eowu::ScriptWrapper::LuaFlipFunction = lua_flip_function;
}

void eowu::ScriptWrapper::SetRenderFunctions(eowu::LuaFunctionContainerType render_functions) {
  eowu::ScriptWrapper::render_functions = std::move(render_functions);
}

void eowu::ScriptWrapper::SetFlipFunctions(eowu::LuaFunctionContainerType flip_functions) {
  eowu::ScriptWrapper::flip_functions = std::move(flip_functions);
}

void eowu::ScriptWrapper::SetActiveRenderFunction(const std::string &id) {
  const auto &it = render_functions->find(id);
  
  if (it == render_functions->end()) {
    throw eowu::NonexistentResourceError("Render function: '" + id + "' does not exist.");
  }
  
  //  make sure the render function has been called at least once
  while (!LuaRenderFunction->DidCall()) {
    EOWU_LOG_WARN("ScriptWrapper::SetActiveRenderFunction: Waiting for render function to be called.");
  }
  
  LuaRenderFunction->Set(it->second);
}

void eowu::ScriptWrapper::SetActiveFlipFunction(const std::string &id) {
  const auto &it = flip_functions->find(id);
  
  if (it == flip_functions->end()) {
    throw eowu::NonexistentResourceError("Flip function: '" + id + "' does not exist.");
  }
  
  LuaFlipFunction->Set(it->second);
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
  
  return states->at(id).get();
}

eowu::RendererWrapper eowu::ScriptWrapper::GetRendererWrapper() const {
  assert(IsComplete());
  
  auto renderer = pipeline->GetRenderer();
  eowu::RendererWrapper render_wrapper(renderer);
  
  return render_wrapper;
}

void eowu::ScriptWrapper::CommitData() const {
  const auto &state_container = *states.get();
  
  eowu::serialize::ByteArrayType into;
  
  for (const auto &it : state_container) {
    const auto &state = it.second;
    
    state->Write(into);
  }
  
  task_data_store->Write(into);
}

void eowu::ScriptWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginClass<eowu::ScriptWrapper>(eowu::constants::eowu_script_name)
  .addConstructor<void(*)(void)>()
  .addFunction("Commit", &eowu::ScriptWrapper::CommitData)
  .addFunction("Stimulus", &eowu::ScriptWrapper::GetModelWrapper)
  .addFunction("State", &eowu::ScriptWrapper::GetStateWrapper)
  .addFunction("Render", &eowu::ScriptWrapper::SetActiveRenderFunction)
  .addFunction("Flip", &eowu::ScriptWrapper::SetActiveFlipFunction)
  .addFunction("Renderer", &eowu::ScriptWrapper::GetRendererWrapper)
  .endClass();
}
