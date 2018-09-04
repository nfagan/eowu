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

void eowu::init::init_render_pipeline(std::shared_ptr<eowu::LuaContext> lua_context,
                                      std::shared_ptr<eowu::LuaFunction> lua_render_function,
                                      std::shared_ptr<eowu::GLPipeline> pipeline) {
  
  lua_State *L = lua_context->GetState();
  
  eowu::GLPipelineWrapper::CreateLuaSchema(L);
  eowu::RendererWrapper::CreateLuaSchema(L);
  eowu::VectorWrapper::CreateLuaSchema(L);
  eowu::ModelWrapper::CreateLuaSchema(L);
  
  eowu::GLPipelineWrapper::Pipeline = pipeline;
  eowu::GLPipelineWrapper::LuaRenderFunction = lua_render_function;
  
}

void eowu::init::init_states(std::shared_ptr<eowu::LuaContext> lua_context,
                             eowu::StateManager &state_manager,
                             const eowu::schema::States &state_schema) {
  
  lua_State *L = lua_context->GetState();
  
  eowu::StateWrapper::CreateLuaSchema(L);
  eowu::TaskWrapper::CreateLuaSchema(L);
  
  eowu::TaskWrapper::States = init::get_states(state_schema, lua_context, state_manager);
}

eowu::StateWrapperContainerType eowu::init::get_states(const eowu::schema::States &schemas,
                                                       std::shared_ptr<eowu::LuaContext> lua_context,
                                                       eowu::StateManager &state_manager) {
  
  auto result = std::make_unique<eowu::StateWrapperMapType>();
  
  lua_State *L = lua_context->GetState();
  
  eowu::StateWrapper::CreateLuaSchema(L);
  
  for (const auto &it : schemas.mapping) {
    const auto &schema = it.second;
    
    auto state = state_manager.CreateState(schema.state_id);
    
    if (schema.duration > 0) {
      state->SetDuration(std::chrono::milliseconds((int)schema.duration));
    }
    
    eowu::LuaReferenceContainer ref_entry(schema.entry_function);
    eowu::LuaReferenceContainer ref_exit(schema.exit_function);
    eowu::LuaReferenceContainer ref_loop(schema.loop_function);
    
    eowu::LuaFunction entry(ref_entry);
    eowu::LuaFunction exit(ref_exit);
    eowu::LuaFunction loop(ref_loop);
    
    auto state_functions = std::make_unique<eowu::LuaStateFunctions>(entry, loop, exit);
    auto wrapper = std::make_shared<eowu::StateWrapper>(state, lua_context, std::move(state_functions));
    
    result->emplace(schema.state_id, std::move(wrapper));
  }
  
  return result;
}
