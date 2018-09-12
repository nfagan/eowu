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

void eowu::init::init_render_schema(lua_State *L) {
  eowu::RendererWrapper::CreateLuaSchema(L);
  eowu::VectorWrapper::CreateLuaSchema(L);
  eowu::ModelWrapper::CreateLuaSchema(L);
}

void eowu::init::init_state_schema(lua_State *L) { 
  eowu::StateWrapper::CreateLuaSchema(L);
  eowu::VariableWrapper::CreateLuaSchema(L);
  eowu::VariablesWrapper::CreateLuaSchema(L);
  eowu::MathWrapper::CreateLuaSchema(L);
}

eowu::LuaFunctionContainerType eowu::init::get_render_functions(const eowu::schema::States &schema) {
  auto result = std::make_unique<eowu::LuaFunctionMapType>();
  
  for (const auto &state_it : schema.mapping) {
    for (const auto &func_it : state_it.second.render_functions) {
      eowu::LuaFunction func(func_it.second);
      
      result->emplace(func_it.first, func);
    }
  }
  
  return result;
}

eowu::LuaFunctionContainerType eowu::init::get_flip_functions(const eowu::schema::States &schema) {
  auto result = std::make_unique<eowu::LuaFunctionMapType>();
  
  for (const auto &state_it : schema.mapping) {
    for (const auto &func_it : state_it.second.flip_functions) {
      eowu::LuaFunction func(func_it.second);
      
      result->emplace(func_it.first, func);
    }
  }
  
  return result;
}

eowu::StateWrapperContainerType eowu::init::get_states(const eowu::schema::States &schemas,
                                                       std::shared_ptr<eowu::LuaContext> lua_context,
                                                       eowu::StateManager &state_manager) {
  
  auto result = std::make_unique<eowu::StateWrapperMapType>();
  
  for (const auto &it : schemas.mapping) {
    const auto &schema = it.second;
    
    auto state = state_manager.CreateState(schema.state_id);
    
    if (schema.duration >= 0) {
      state->SetDuration(std::chrono::milliseconds((int)schema.duration));
    }
    
    eowu::LuaFunction entry(schema.entry_function);
    eowu::LuaFunction exit(schema.exit_function);
    eowu::LuaFunction loop(schema.loop_function);
    
    auto state_functions = std::make_unique<eowu::LuaStateFunctions>(entry, loop, exit);
    auto wrapper = std::make_shared<eowu::StateWrapper>(state, schema.variables, lua_context, std::move(state_functions));
    
    result->emplace(schema.state_id, std::move(wrapper));
  }
  
  return result;
}
