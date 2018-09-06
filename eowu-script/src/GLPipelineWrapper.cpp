//
//  GLPipelineWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "GLPipelineWrapper.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include "Constants.hpp"
#include "Lua.hpp"
#include "LuaFunction.hpp"
#include "RendererWrapper.hpp"
#include "ModelWrapper.hpp"
#include <assert.h>

std::shared_ptr<eowu::GLPipeline> eowu::GLPipelineWrapper::Pipeline = nullptr;
std::shared_ptr<eowu::LuaFunction> eowu::GLPipelineWrapper::LuaRenderFunction = nullptr;

eowu::RendererWrapper eowu::GLPipelineWrapper::GetRendererWrapper() {
  assert(Pipeline);
  assert(LuaRenderFunction);
  
  return eowu::RendererWrapper(Pipeline->GetRenderer(), LuaRenderFunction);
}

eowu::ModelWrapper eowu::GLPipelineWrapper::GetModelWrapper(const std::string &id) {
  assert(Pipeline);
  
  auto model = Pipeline->GetResourceManager()->Get<eowu::Model>(id);
  auto renderer = Pipeline->GetRenderer();
  auto texture_manager = Pipeline->GetTextureManager();
  auto window_container = Pipeline->GetWindowContainer();
  
  eowu::ModelWrapper model_wrapper(model, renderer, window_container, texture_manager);
  
  return model_wrapper;
}

void eowu::GLPipelineWrapper::CreateLuaSchema(lua_State *L) {
  using namespace luabridge;
  
  getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::GLPipelineWrapper>("GL")
  .addStaticFunction("Renderer", &eowu::GLPipelineWrapper::GetRendererWrapper)
  .addStaticFunction("Stimulus", &eowu::GLPipelineWrapper::GetModelWrapper)
  .endClass()
  .endNamespace();
}
