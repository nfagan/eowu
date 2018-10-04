//
//  ScriptWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#pragma once

#include "Init.hpp"
#include "TargetWrapper.hpp"
#include "TargetSetWrapper.hpp"
#include "TimeoutWrapper.hpp"
#include <eowu-common/LockedResource.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

struct lua_State;

namespace eowu {
  class GLPipeline;
  class RendererWrapper;
  class KeyboardWrapper;
  class LuaFunction;
  class LuaContext;
  class LockedLuaRenderFunctions;
  class ModelWrapper;
  class StateWrapper;
  class VariableWrapper;
  class XYTarget;
  class StateRunner;
  
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
  void SetTargetWrapperContainer(const std::unordered_map<std::string, std::shared_ptr<eowu::TargetWrapper>> &targets);
  void SetTimeoutWrapperContainer(eowu::TimeoutWrapperContainerType timeout_wrappers);
  void SetXYTargets(const std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> &targets);
  
  void SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline);
  void SetRenderFunctions(eowu::LuaFunctionContainerType render_functions);
  void SetFlipFunctions(eowu::LuaFunctionContainerType render_functions);
  void SetLuaRenderFunctionPair(std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions);
  void SetKeyboardWrapper(std::unique_ptr<eowu::KeyboardWrapper> keyboard);
  void SetLuaTaskContext(std::shared_ptr<eowu::LuaContext> context);
  void SetLuaRenderContext(std::shared_ptr<eowu::LuaContext> context);
  
  void SetVariables(const std::unordered_map<std::string, eowu::data::Commitable> &variables);
  void SetTaskDataStore(std::shared_ptr<eowu::data::Store> task_data_store);
  void SetLockedRenderFunctions(std::shared_ptr<eowu::LockedLuaRenderFunctions> locked_functions);
  void SetThreadIds(const std::thread::id &render, const std::thread::id &task);
  void SetStateRunner(eowu::StateRunner *runner);
  
  eowu::TargetSetWrapper* MakeTargetSet(const std::string &id, lua_State *L);
  eowu::TimeoutWrapper* MakeTimeout(const std::string &id, int ms, luabridge::LuaRef func);
  
  bool IsComplete() const;
  
  int SetRenderFunctionPair(lua_State *L);
  eowu::StateWrapper* GetStateWrapper(const std::string &id) const;
  eowu::KeyboardWrapper* GetKeyboardWrapper() const;
  eowu::TargetWrapper* GetTargetWrapper(const std::string &id);
  eowu::TimeoutWrapper* GetTimeoutWrapper(const std::string &id);
  eowu::RendererWrapper GetRendererWrapper() const;
  eowu::ModelWrapper GetModelWrapper(const std::string &id) const;
  eowu::VariableWrapper GetVariable(const std::string &id);
  
  double GetEllapsedTime() const;
  void Exit();
  
  std::shared_ptr<eowu::LockedLuaRenderFunctions> GetLockedRenderFunctions() const;
  
  std::size_t CountVariables() const;
  void CommitData() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  struct ThreadIds {
    std::thread::id render;
    std::thread::id task;
  };
  
  struct Variables {
    std::unordered_map<std::string, eowu::data::Commitable> active;
    std::unordered_map<std::string, eowu::data::Commitable> defaults;
  };
  
  struct LuaContexts {
    std::shared_ptr<eowu::LuaContext> task;
    std::shared_ptr<eowu::LuaContext> render;
  };
  
  static std::unordered_map<std::string, std::shared_ptr<eowu::TargetWrapper>> target_wrappers;
  static eowu::TimeoutWrapperContainerType timeout_wrappers;
  static eowu::LuaFunctionContainerType render_functions;
  static eowu::LuaFunctionContainerType flip_functions;
  static eowu::StateWrapperContainerType states;
  static std::shared_ptr<eowu::GLPipeline> pipeline;
  static Variables variables;
  static std::shared_ptr<eowu::data::Store> task_data_store;
  static std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_thread_functions;
  static std::unordered_map<std::string, std::unique_ptr<eowu::TargetSetWrapper>> target_sets;
  static std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> xy_targets;
  static std::unique_ptr<eowu::KeyboardWrapper> keyboard;
  static ThreadIds thread_ids;
  static eowu::StateRunner *state_runner;
  static LuaContexts lua_contexts;
  
  void commit_variables(std::vector<char> &into) const;
  void commit_states(std::vector<char> &into) const;
  
  eowu::LuaFunction* get_function_from_state(lua_State *L,
                                             int stack_index,
                                             eowu::LuaFunctionMapType *funcs,
                                             const std::string &kind);
  
  bool is_render_thread();
  bool is_task_thread();
};
