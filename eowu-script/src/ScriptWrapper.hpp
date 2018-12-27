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
  class XYSourceWrapper;
  class XYTarget;
  class XYSource;
  class StateRunner;
  class TimeoutHandleWrapper;
  class TextModelWrapper;
  
  class TextModel;
  
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
  ScriptWrapper() = delete;
  ~ScriptWrapper() = delete;
  
  static void SetStateWrapperContainer(eowu::StateWrapperContainerType states);
  static void SetTargetWrapperContainer(const std::unordered_map<std::string,
                                        std::shared_ptr<eowu::TargetWrapper>> &targets);
  static void SetXYTargets(const std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> &targets);
  static void SetXYSources(const std::unordered_map<std::string, const eowu::XYSource*> &sources);
  
  static void SetGLPipeline(std::shared_ptr<eowu::GLPipeline> pipeline);
  static void SetTextModels(const std::unordered_map<std::string, std::shared_ptr<eowu::TextModel>> &text_models);
  static void SetAudioContext(std::shared_ptr<eowu::AudioContext> context);
  static void SetRenderFunctions(eowu::LuaFunctionContainerType render_functions);
  static void SetFlipFunctions(eowu::LuaFunctionContainerType render_functions);
  static void SetLuaRenderFunctionPair(std::shared_ptr<eowu::LockedLuaRenderFunctions> lua_render_functions);
  static void SetKeyboardWrapper(std::unique_ptr<eowu::KeyboardWrapper> keyboard);
  static void SetLuaTaskContext(std::shared_ptr<eowu::LuaContext> context);
  static void SetLuaRenderContext(std::shared_ptr<eowu::LuaContext> context);
  
  static void SetVariables(const std::unordered_map<std::string, eowu::data::Commitable> &variables);
  static void SetTaskDataStore(std::shared_ptr<eowu::data::Store> task_data_store);
  static void SetLockedRenderFunctions(std::shared_ptr<eowu::LockedLuaRenderFunctions> locked_functions);
  static void SetThreadIds(const std::thread::id &render, const std::thread::id &task);
  static void SetStateRunner(eowu::StateRunner *runner);
  static void SetSounds(const std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>> &sounds);
  
  static int SetRenderFunctionPair(lua_State *L);
  
  static eowu::TargetSetWrapper MakeTargetSet(const std::string &id, lua_State *L);
  static eowu::TimeoutHandleWrapper MakeTimeout(const std::string &id, int ms, luabridge::LuaRef func);
  static eowu::TimeoutHandleWrapper MakeInterval(const std::string &id, int ms, luabridge::LuaRef func);
  static eowu::ModelWrapper MakeModelWrapper(const std::string &id);
  
  static bool IsComplete();
  
  static const eowu::TimeoutAggregateMapType* GetIntervalWrappers();
  static const eowu::TimeoutAggregateMapType* GetTimeoutWrappers();
  
  static eowu::StateWrapper* GetStateWrapper(const std::string &id);
  static eowu::StateWrapper* GetActiveStateWrapper();
  static eowu::KeyboardWrapper* GetKeyboardWrapper();
  static eowu::TargetWrapper* GetTargetWrapper(const std::string &id);
  static eowu::TargetSetWrapper GetTargetSetWrapper(const std::string &id);
  static eowu::TimeoutHandleWrapper GetTimeoutHandleWrapper(const std::string &id);
  static eowu::TimeoutHandleWrapper GetIntervalHandleWrapper(const std::string &id);
  static eowu::RendererWrapper GetRendererWrapper();
  static eowu::ModelWrapper GetModelWrapper(const std::string &id);
  static eowu::TextModelWrapper GetTextModelWrapper(const std::string &id);
  static eowu::VariableWrapper GetVariable(const std::string &id);
  static eowu::WindowWrapper GetWindowWrapper(const std::string &id);
  static eowu::AudioSourceWrapper GetAudioSourceWrapper(const std::string &id);
  static eowu::XYSourceWrapper GetXYSourceWrapper(const std::string &id);
  
  static double GetElapsedTime();
  static void Exit();
  
  static std::shared_ptr<eowu::LockedLuaRenderFunctions> GetLockedRenderFunctions();
  
  static std::size_t CountVariables();
  static void CommitData();
  
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
  static std::unordered_map<std::string, std::shared_ptr<eowu::XYTargetSet>> target_sets;
  static std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>> xy_targets;
  static std::unordered_map<std::string, const eowu::XYSource*> xy_sources;
  static std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>> sounds;
  static std::unordered_map<std::string, std::shared_ptr<eowu::TextModel>> text_models;
  static std::unique_ptr<eowu::KeyboardWrapper> keyboard;
  static ThreadIds thread_ids;
  static eowu::StateRunner *state_runner;
  static LuaContexts lua_contexts;
  
  static eowu::TimeoutHandleWrapper make_timeout(eowu::TimeoutAggregateMapType *aggregate,
                                                 const std::string &id,
                                                 int ms,
                                                 const luabridge::LuaRef &func,
                                                 const char* const make_func_id,
                                                 eowu::Timeout::Type timeout_type);
  
  static eowu::TimeoutHandleWrapper get_timeout_handle_wrapper(const std::string &id,
                                                               const eowu::TimeoutAggregateMapType *aggregate,
                                                               const char* const kind);
  
  static void commit_variables(std::vector<char> &into);
  static void commit_states(std::vector<char> &into);
  
  static eowu::LuaFunction* get_function_from_state(lua_State *L,
                                                    int stack_index,
                                                    eowu::LuaFunctionMapType *funcs,
                                                    const std::string &kind);
  
  static std::shared_ptr<eowu::LuaContext> get_lua_context_for_thread();
  
  static bool is_render_thread();
  static bool is_task_thread();
};
