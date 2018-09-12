//
//  StateWrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateWrapper.hpp"
#include "VariableWrapper.hpp"
#include "Constants.hpp"
#include "Error.hpp"
#include <eowu-state/eowu-state.hpp>
#include <chrono>

eowu::LuaStateFunctions::LuaStateFunctions(const eowu::LuaFunction &entry,
                                           const eowu::LuaFunction &loop,
                                           const eowu::LuaFunction &exit):
on_entry(entry), on_loop(loop), on_exit(exit) {
  //
}

eowu::LuaStateFunctions::LuaStateFunctions(const LuaStateFunctions &other) :
on_entry(other.on_entry), on_loop(other.on_loop), on_exit(other.on_exit) {
  //
}

eowu::StateWrapper::StateWrapper(eowu::State *state_,
                                 const std::unordered_map<std::string, eowu::data::Commitable> &variables_,
                                 std::shared_ptr<eowu::LuaContext> context,
                                 std::unique_ptr<eowu::LuaStateFunctions> state_functions_) :
state(state_), active_variables(variables_), default_variables(variables_),
lua_context(context), state_functions(std::move(state_functions_)) {
  setup_state_callbacks();
  setup_variable_wrappers();
}

eowu::VariableWrapper* eowu::StateWrapper::GetVariable(const std::string &name) {
  auto it = variable_wrappers.find(name);
  
  if (it == variable_wrappers.end()) {
    throw eowu::LuaError("Reference to non-existent variable: '" + name + "'.");
  }
  
  return &it->second;
}

void eowu::StateWrapper::Write(eowu::serialize::ByteArrayType &into) const {
  eowu::u64 sz = 0;
  
  for (const auto &it : active_variables) {
    if (it.second.IsCommitted()) {
      sz += 1;
    }
  }
  
  //  nest variables under the state id namespace
  eowu::serialize::unsafe_nest_aggregate(state->GetId(), 1, into);
  eowu::serialize::unsafe_nest_aggregate(eowu::constants::eowu_variables_name, sz, into);
  
  for (const auto &it : active_variables) {
    const auto &var = it.second;
    
    //  only insert data if it has been committed.
    if (!var.IsCommitted()) {
      continue;
    }
    
    const auto inserter = [&](const auto &data) -> void {
      eowu::serialize::serialize(data, into);
    };
    
    var.Use(inserter);
  }
}

double eowu::StateWrapper::Ellapsed() const {
  return state->GetTimer().Ellapsed().count();
}

void eowu::StateWrapper::Exit() {
  state->Exit();
}

void eowu::StateWrapper::SetDuration(int duration) {
  state->SetDuration(std::chrono::milliseconds(duration));
}

void eowu::StateWrapper::SetNextState(const std::string &next) {
  state->Next(state->GetState(next));
}

void eowu::StateWrapper::setup_variable_wrappers() {
  for (auto &it : active_variables) {
    const std::string &id = it.first;
    auto *active_value = &it.second;
    auto *default_value = &default_variables.at(id);
    
    eowu::VariableWrapper wrapper(active_value, default_value);
    
    variable_wrappers.emplace(id, wrapper);
  }
}

void eowu::StateWrapper::setup_state_callbacks() {
  //  entry
  state->SetOnEntry([&](auto* state) {
    lua_context->Call(state_functions->on_entry);
  });
  
  //  loop
  state->SetOnLoop([&](auto* state) {
    lua_context->Call(state_functions->on_loop);
  });
  
  //  exit
  state->SetOnExit([&](auto* state) {
    lua_context->Call(state_functions->on_exit);
  });
}

void eowu::StateWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::StateWrapper>("_State")
  .addFunction("Variable", &eowu::StateWrapper::GetVariable)
  .addFunction("Duration", &eowu::StateWrapper::SetDuration)
  .addFunction("Next", &eowu::StateWrapper::SetNextState)
  .addFunction("Exit", &eowu::StateWrapper::Exit)
  .addFunction("Ellapsed", &eowu::StateWrapper::Ellapsed)
  .endClass()
  .endNamespace();
}
