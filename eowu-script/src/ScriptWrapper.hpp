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
  class LockedLuaRenderFunctions;
  class ModelWrapper;
  class StateWrapper;
  
  namespace data {
    class Store;
  }
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
  void SetRenderFunctions(eowu::LuaFunctionContainerType render_functions);
  void SetFlipFunctions(eowu::LuaFunctionContainerType render_functions);
  void SetLuaRenderFunctionPair(std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions);
  
  bool IsComplete() const;
  
  void SetRenderFunctionPair(const std::string &render_id, const std::string &flip_id);
  eowu::StateWrapper* GetStateWrapper(const std::string &id) const;
  eowu::RendererWrapper GetRendererWrapper() const;
  eowu::ModelWrapper GetModelWrapper(const std::string &id) const;
  
  void CommitData() const;
  
  static std::shared_ptr<eowu::LockedLuaRenderFunctions> LuaRenderThreadFunctions;
  //  public for now
  static std::shared_ptr<eowu::data::Store> task_data_store;
  
  static void CreateLuaSchema(lua_State *L);
private:
  static eowu::LuaFunctionContainerType render_functions;
  static eowu::LuaFunctionContainerType flip_functions;
  static eowu::StateWrapperContainerType states;
  static std::shared_ptr<eowu::GLPipeline> pipeline;
};
