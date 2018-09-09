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
state(state_), variables(variables_), lua_context(context), state_functions(std::move(state_functions_)) {
  setup_state_callbacks();
}

eowu::VariableWrapper eowu::StateWrapper::GetVariable(const std::string &name) {
  auto it = variables.find(name);
  
  if (it == variables.end()) {
    throw eowu::LuaError("Reference to non-existent variable: '" + name + "'.");
  }
  
  eowu::data::Commitable* variable = &it->second;
  eowu::VariableWrapper wrapper(variable);
  
  return wrapper;
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
