//
//  GLPipelineWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>

struct lua_State;

namespace eowu {
  class GLPipelineWrapper;
  class GLPipeline;
  class RendererWrapper;
  class LuaFunction;
  class ModelWrapper;
}

class eowu::GLPipelineWrapper {
public:
  static std::shared_ptr<eowu::GLPipeline> Pipeline;
  static std::shared_ptr<eowu::LuaFunction> LuaRenderFunction;
  
  static eowu::RendererWrapper GetRendererWrapper();
  static eowu::ModelWrapper GetModelWrapper(const std::string &id);
  
  static void CreateLuaSchema(lua_State *L);
};
