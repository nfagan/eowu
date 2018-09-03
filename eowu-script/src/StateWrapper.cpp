//
//  StateWrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "StateWrapper.hpp"
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
                                 std::shared_ptr<eowu::LuaContext> context,
                                 std::unique_ptr<eowu::LuaStateFunctions> state_functions_) :
state(state_), lua_context(context), state_functions(std::move(state_functions_)) {
  setup_state_callbacks();
}

double eowu::StateWrapper::EllapsedSeconds() const {
  return state->GetTimer().Ellapsed().count();
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
  .beginNamespace("eowu")
  .beginClass<eowu::StateWrapper>("_State")
  .addFunction("Duration", &eowu::StateWrapper::SetDuration)
  .addFunction("Next", &eowu::StateWrapper::SetNextState)
  .addFunction("Ellapsed", &eowu::StateWrapper::EllapsedSeconds)
  .endClass()
  .endNamespace();
}
