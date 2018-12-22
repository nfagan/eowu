//
//  LuaInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/22/18.
//

#pragma once

#include "SetupResult.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <string>
#include <memory>

struct lua_State;

namespace eowu {
  
  class StateRunner;
  class StateManager;
  
  namespace init {
    struct LuaContexts {
      std::shared_ptr<eowu::LuaContext> task = nullptr;
      std::shared_ptr<eowu::LuaContext> render = nullptr;
    };
    
    struct ScriptWrapper {
      eowu::State *first_state = nullptr;
    };
    
    using SchemaResult = eowu::SetupResult<eowu::schema::Setup>;
    using ContextResult = eowu::SetupResult<eowu::init::LuaContexts>;
    using ScriptWrapperResult = eowu::SetupResult<eowu::init::ScriptWrapper>;
    
    ContextResult initialize_lua_contexts();
    
    SchemaResult initialize_schema(const std::string &file,
                                   std::shared_ptr<eowu::LuaContext> task_context);
    
    ScriptWrapperResult initialize_script_wrapper(const std::string &file,
                                                  const eowu::schema::Setup &schema,
                                                  std::shared_ptr<eowu::GLPipeline> gl_pipeline,
                                                  eowu::init::LuaContexts lua_contexts,
                                                  eowu::StateManager &state_manager,
                                                  eowu::StateRunner &state_runner);
    
    eowu::schema::Setup parse_setup_schema(const std::string &file,
                                           lua_State *lua_task_state);
    
    void validate_setup_schema(const eowu::schema::Setup &schema);
    void insert_lua_callbacks(lua_State *L);
    
    std::shared_ptr<eowu::LuaContext> create_lua_context();
    std::unique_ptr<eowu::KeyboardWrapper> create_keyboard(eowu::Keyboard &keyboard);
    
    eowu::State* try_get_first_state(const eowu::schema::States &schema,
                                     const eowu::StateManager &stage_manager);
    
    eowu::StateWrapperContainerType get_states(const eowu::schema::States &schema,
                                               std::shared_ptr<eowu::LuaContext> lua_context,
                                               eowu::StateManager &state_manager,
                                               eowu::StateRunner &state_runner);
    
    eowu::LuaFunctionContainerType get_render_functions(const eowu::schema::States &schema);
    eowu::LuaFunctionContainerType get_flip_functions(const eowu::schema::States &schema);
    
    eowu::parser::ParseResult<eowu::schema::States> get_render_state_schema(const std::string &file,
                                                                            lua_State *render_state);
    
    void init_state_schema(lua_State *L);
    void init_render_schema(lua_State *L);
    void init_static_schemas(lua_State *L);
    void init_audio_schemas(lua_State *L);
  }
}
