//
//  ScriptWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#pragma once

#include "Init.hpp"
#include <memory>
#include <string>

struct lua_State;

namespace eowu {
  class GLPipeline;
  class RendererWrapper;
  class LuaFunction;
  class ModelWrapper;
  class StateWrapper;
}

struct lua_State;

namespace eowu {
  class ScriptWrapper;
}

class eowu::ScriptWrapper {
public:
  ScriptWrapper() = default;
  ~ScriptWrapper() = default;
  
  void SetStateWrapperContainer(eowu::StateWrapperContainerType states);
  void SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline);
  void SetRenderFunctions(eowu::RenderFunctionContainerType render_functions);
  void SetLuaRenderFunction(std::shared_ptr<eowu::LuaFunction> lua_render_function);
  
  bool IsComplete() const;
  
  void SetActiveRenderFunction(const std::string &id);
  eowu::StateWrapper* GetStateWrapper(const std::string &id) const;
  eowu::RendererWrapper GetRendererWrapper() const;
  eowu::ModelWrapper GetModelWrapper(const std::string &id) const;
  
  static std::shared_ptr<eowu::LuaFunction> LuaRenderFunction;
  
  static void CreateLuaSchema(lua_State *L);
private:
  static eowu::RenderFunctionContainerType render_functions;
  static eowu::StateWrapperContainerType states;
  static std::shared_ptr<eowu::GLPipeline> pipeline;
};
