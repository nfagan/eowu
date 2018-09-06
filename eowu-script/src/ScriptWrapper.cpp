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
#include <eowu-gl/eowu-gl.hpp>
#include <assert.h>

eowu::StateWrapperContainerType eowu::ScriptWrapper::states = nullptr;
std::shared_ptr<eowu::GLPipeline> eowu::ScriptWrapper::pipeline = nullptr;
eowu::RenderFunctionContainerType eowu::ScriptWrapper::render_functions = nullptr;
std::shared_ptr<eowu::LuaFunction> eowu::ScriptWrapper::LuaRenderFunction = nullptr;

bool eowu::ScriptWrapper::IsComplete() const {
  return states != nullptr && pipeline != nullptr && render_functions != nullptr;
}

void eowu::ScriptWrapper::SetStateWrapperContainer(eowu::StateWrapperContainerType states) {
  eowu::ScriptWrapper::states = std::move(states);
}

void eowu::ScriptWrapper::SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline) {
  eowu::ScriptWrapper::pipeline = pipeline;
}

void eowu::ScriptWrapper::SetRenderFunctions(eowu::RenderFunctionContainerType render_functions) {
  eowu::ScriptWrapper::render_functions = std::move(render_functions);
}

void eowu::ScriptWrapper::SetActiveRenderFunction(const std::string &id) {
  const auto &it = render_functions->find(id);
  
  if (it == render_functions->end()) {
    throw eowu::NonexistentResourceError("Render function: '" + id + "' does not exist.");
  }
  
  LuaRenderFunction->Set(it->second);
}

eowu::ModelWrapper eowu::ScriptWrapper::GetModelWrapper(const std::string &id) const {
  assert(IsComplete());
    
  auto model = pipeline->GetResourceManager()->Get<eowu::Model>(id);
  auto renderer = pipeline->GetRenderer();
  auto texture_manager = pipeline->GetTextureManager();
  
  eowu::ModelWrapper model_wrapper(model, renderer, texture_manager);
  
  return model_wrapper;
}

eowu::StateWrapper* eowu::ScriptWrapper::GetStateWrapper(const std::string &id) const {
  assert(IsComplete());
  
  return states->at(id).get();
}

void eowu::ScriptWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginClass<eowu::ScriptWrapper>(eowu::constants::eowu_script_name)
  .addConstructor<void(*)(void)>()
  .addFunction("Stimulus", &eowu::ScriptWrapper::GetModelWrapper)
  .addFunction("State", &eowu::ScriptWrapper::GetStateWrapper)
  .addFunction("Render", &eowu::ScriptWrapper::SetActiveRenderFunction)
  .endClass();
}
