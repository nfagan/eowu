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
#include "TimeoutAggregate.hpp"
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
  class WindowWrapper;
  class XYTarget;
  class StateRunner;
  class TimeoutHandleWrapper;
  
  class AudioContext;
  class AudioBufferSource;
  class AudioSourceWrapper;
  
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
  void SetXYTargets(const std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> &targets);
  
  void SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline);
  void SetAudioContext(std::shared_ptr<eowu::AudioContext> context);
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
  void SetSounds(const std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>> &sounds);
  
  int SetRenderFunctionPair(lua_State *L);
  
  eowu::TargetSetWrapper* MakeTargetSet(const std::string &id, lua_State *L);
  eowu::TimeoutHandleWrapper MakeTimeout(const std::string &id, int ms, luabridge::LuaRef func);
  eowu::TimeoutHandleWrapper MakeInterval(const std::string &id, int ms, luabridge::LuaRef func);
  
  bool IsComplete() const;
  
  const eowu::TimeoutAggregateMapType* GetIntervalWrappers() const;
  const eowu::TimeoutAggregateMapType* GetTimeoutWrappers() const;
  
  eowu::StateWrapper* GetStateWrapper(const std::string &id) const;
  eowu::StateWrapper* GetActiveStateWrapper() const;
  eowu::KeyboardWrapper* GetKeyboardWrapper() const;
  eowu::TargetWrapper* GetTargetWrapper(const std::string &id);
  eowu::TargetSetWrapper* GetTargetSetWrapper(const std::string &id);
  eowu::TimeoutHandleWrapper GetTimeoutHandleWrapper(const std::string &id);
  eowu::TimeoutHandleWrapper GetIntervalHandleWrapper(const std::string &id);
  eowu::RendererWrapper GetRendererWrapper() const;
  eowu::ModelWrapper GetModelWrapper(const std::string &id) const;
  eowu::VariableWrapper GetVariable(const std::string &id);
  eowu::WindowWrapper GetWindowWrapper(const std::string &id);
  eowu::AudioSourceWrapper GetAudioSourceWrapper(const std::string &id) const;
  
  double GetElapsedTime() const;
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
  static eowu::TimeoutAggregateMapType timeout_wrappers;
  static eowu::TimeoutAggregateMapType interval_wrappers;
  static eowu::LuaFunctionContainerType render_functions;
  static eowu::LuaFunctionContainerType flip_functions;
  static eowu::StateWrapperContainerType states;
  static std::shared_ptr<eowu::GLPipeline> pipeline;
  static std::shared_ptr<eowu::AudioContext> audio_context;
  static Variables variables;
  static std::shared_ptr<eowu::data::Store> task_data_store;
  static std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_thread_functions;
  static std::unordered_map<std::string, std::unique_ptr<eowu::TargetSetWrapper>> target_sets;
  static std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> xy_targets;
  static std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>> sounds;
  static std::unique_ptr<eowu::KeyboardWrapper> keyboard;
  static ThreadIds thread_ids;
  static eowu::StateRunner *state_runner;
  static LuaContexts lua_contexts;
  
  eowu::TimeoutHandleWrapper make_timeout(eowu::TimeoutAggregateMapType *aggregate,
                                          const std::string &id,
                                          int ms,
                                          const luabridge::LuaRef &func,
                                          const char* const make_func_id,
                                          eowu::Timeout::Type timeout_type);
  
  eowu::TimeoutHandleWrapper get_timeout_handle_wrapper(const std::string &id,
                                                        const eowu::TimeoutAggregateMapType *aggregate,
                                                        const char* const kind);
  
  void commit_variables(std::vector<char> &into) const;
  void commit_states(std::vector<char> &into) const;
  
  eowu::LuaFunction* get_function_from_state(lua_State *L,
                                             int stack_index,
                                             eowu::LuaFunctionMapType *funcs,
                                             const std::string &kind);
  
  std::shared_ptr<eowu::LuaContext> get_lua_context_for_thread();
  
  bool is_render_thread();
  bool is_task_thread();
};
