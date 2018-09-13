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
#include <vector>

struct lua_State;

namespace eowu {
  class GLPipeline;
  class RendererWrapper;
  class LuaFunction;
  class LockedLuaRenderFunctions;
  class ModelWrapper;
  class StateWrapper;
  class VariableWrapper;
  
  namespace data {
    class Store;
    class Commitable;
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
  void SetVariables(const std::unordered_map<std::string, eowu::data::Commitable> &variables);
  void SetTaskDataStore(std::shared_ptr<eowu::data::Store> task_data_store);
  void SetLockedRenderFunctions(std::shared_ptr<eowu::LockedLuaRenderFunctions> locked_functions);
  
  bool IsComplete() const;
  
  void SetRenderFunctionPair(const std::string &render_id, const std::string &flip_id);
  eowu::StateWrapper* GetStateWrapper(const std::string &id) const;
  eowu::RendererWrapper GetRendererWrapper() const;
  eowu::ModelWrapper GetModelWrapper(const std::string &id) const;
  eowu::VariableWrapper GetVariable(const std::string &id);
  
  std::shared_ptr<eowu::LockedLuaRenderFunctions> GetLockedRenderFunctions() const;
  
  std::size_t CountVariables() const;
  
  void CommitData() const;
  
  static void CreateLuaSchema(lua_State *L);
  
  struct Variables {
    std::unordered_map<std::string, eowu::data::Commitable> active;
    std::unordered_map<std::string, eowu::data::Commitable> defaults;
  };
private:
  static eowu::LuaFunctionContainerType render_functions;
  static eowu::LuaFunctionContainerType flip_functions;
  static eowu::StateWrapperContainerType states;
  static std::shared_ptr<eowu::GLPipeline> pipeline;
  static Variables variables;
  static std::shared_ptr<eowu::data::Store> task_data_store;
  static std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_thread_functions;
  
  void commit_variables(std::vector<char> &into) const;
  void commit_states(std::vector<char> &into) const;
};
