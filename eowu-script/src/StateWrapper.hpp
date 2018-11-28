//
//  StateWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include "LuaFunction.hpp"
#include "LuaContext.hpp"
#include <eowu-data.hpp>
#include <memory>
#include <unordered_map>

namespace eowu {
  class StateWrapper;
  class VariableWrapper;
  class State;
  struct LuaStateFunctions;
}

struct eowu::LuaStateFunctions {
  eowu::LuaFunction on_entry;
  eowu::LuaFunction on_loop;
  eowu::LuaFunction on_exit;
  
  LuaStateFunctions(const eowu::LuaFunction &entry,
                    const eowu::LuaFunction &loop,
                    const eowu::LuaFunction &exit);
  
  LuaStateFunctions(const LuaStateFunctions &other);
};

class eowu::StateWrapper {
public:
  StateWrapper(eowu::State *state,
               const std::unordered_map<std::string, eowu::data::Commitable> &variables,
               std::shared_ptr<eowu::LuaContext> context,
               std::unique_ptr<eowu::LuaStateFunctions> state_functions);
  
  eowu::VariableWrapper* GetVariable(const std::string &name);
  
  void Write(eowu::serialize::ByteArrayType &into) const;
  
  void SetDuration(int duration);
  void SetNextState(const std::string &next);
  double Elapsed() const;
  int Exit(lua_State *L);
  
  const std::string& GetName() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  
  eowu::State *state;
  std::unordered_map<std::string, eowu::data::Commitable> active_variables;
  std::unordered_map<std::string, eowu::data::Commitable> default_variables;
  std::unordered_map<std::string, eowu::VariableWrapper> variable_wrappers;
  
  std::shared_ptr<eowu::LuaContext> lua_context;
  std::unique_ptr<eowu::LuaStateFunctions> state_functions;
  
  void setup_state_callbacks();
  void setup_variable_wrappers();
  
  eowu::data::Struct get_latest_timing_info() const;
};
