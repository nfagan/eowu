//
//  init.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "init.hpp"
#include "Lua.hpp"
#include "LuaContext.hpp"
#include "Wrappers.hpp"
#include "../parser/Schemas.hpp"
#include <eowu-state/eowu-state.hpp>
#include <chrono>

#define EOWU_GET_LUA_FUNCTION_CONTAINER(name, field) \
  eowu::LuaFunctionContainerType eowu::init::name(const eowu::schema::States &schema) { \
    auto result = std::make_unique<eowu::LuaFunctionMapType>(); \
    \
    for (const auto &state_it : schema.mapping) { \
      for (const auto &func_it : state_it.second.field) { \
        eowu::LuaFunction func(func_it.second); \
        result->emplace(func_it.first, func); \
      } \
    } \
      \
    return result; \
  }

void eowu::init::init_static_schemas(lua_State *L) {
  eowu::FilesystemWrapper::CreateLuaSchema(L);
}

void eowu::init::init_render_schema(lua_State *L) {
  eowu::RendererWrapper::CreateLuaSchema(L);
  eowu::VectorWrapper::CreateLuaSchema(L);
  eowu::ModelWrapper::CreateLuaSchema(L);
  eowu::KeyboardWrapper::CreateLuaSchema(L);
}

void eowu::init::init_state_schema(lua_State *L) { 
  eowu::StateWrapper::CreateLuaSchema(L);
  eowu::VariableWrapper::CreateLuaSchema(L);
  eowu::VariablesWrapper::CreateLuaSchema(L);
  eowu::MathWrapper::CreateLuaSchema(L);
  eowu::TargetWrapper::CreateLuaSchema(L);
  eowu::TargetSetWrapper::CreateLuaSchema(L);
  eowu::TimeoutWrapper::CreateLuaSchema(L);
}

EOWU_GET_LUA_FUNCTION_CONTAINER(get_render_functions, render_functions)
EOWU_GET_LUA_FUNCTION_CONTAINER(get_flip_functions, flip_functions)

eowu::State* eowu::init::get_first_state(const eowu::schema::States &schema,
                                         const eowu::StateManager &state_manager) {
  
  for (const auto &it : schema.mapping) {
    const auto &state = it.second;
    
    if (state.is_first) {
      return state_manager.GetState(state.state_id);
    }
  }
  
  const auto keys = state_manager.GetStateIds();
  
  if (keys.size() == 0) {
    EOWU_LOG_WARN("init::get_first_state: No states were present.");
    return nullptr;
  }
  
  const auto &state_id = keys[0];
  EOWU_LOG_WARN("init::get_first_state: No state was marked as first; using '" + state_id + "'.");
  
  return state_manager.GetState(state_id);
}

eowu::StateWrapperContainerType eowu::init::get_states(const eowu::schema::States &schemas,
                                                       std::shared_ptr<eowu::LuaContext> lua_context,
                                                       eowu::StateManager &state_manager,
                                                       eowu::StateRunner &state_runner) {
  
  auto result = std::make_unique<eowu::StateWrapperMapType>();
  
  for (const auto &it : schemas.mapping) {
    const auto &schema = it.second;
    
    auto state = state_manager.CreateState(schema.state_id);
    
    if (schema.duration >= 0) {
      state->SetDuration(std::chrono::milliseconds((int)schema.duration));
    }
    
    state->SetGlobalTimer(&state_runner.GetTimer());
    
    eowu::LuaFunction entry(schema.entry_function);
    eowu::LuaFunction exit(schema.exit_function);
    eowu::LuaFunction loop(schema.loop_function);
    
    auto state_functions = std::make_unique<eowu::LuaStateFunctions>(entry, loop, exit);
    auto wrapper = std::make_shared<eowu::StateWrapper>(state, schema.variables, lua_context, std::move(state_functions));
    
    result->emplace(schema.state_id, std::move(wrapper));
  }
  
  return result;
}
