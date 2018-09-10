//
//  LuaRuntime.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "SetupStatus.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-state/eowu-state.hpp>
#include <string>
#include <memory>

struct lua_State;

namespace eowu {
  class LuaRuntime;
}

class eowu::LuaRuntime {
  
public:
  LuaRuntime() = default;
  ~LuaRuntime() = default;
  
  bool InitializeSchema(const std::string &file);
  void InitializeScriptWrapper(const std::string &file,
                               std::shared_ptr<eowu::GLPipeline> gl_pipeline);
  
  eowu::State* GetFirstState();
  
  eowu::schema::Setup setup_schema;
  
  struct LuaContexts {
    std::shared_ptr<eowu::LuaContext> task = nullptr;
    std::shared_ptr<eowu::LuaContext> render = nullptr;
  } lua_contexts;
  
private:
  eowu::ScriptWrapper script_wrapper;
  eowu::StateManager state_manager;
  
  void initialize_lua_contexts();
  
  eowu::parser::ParseResult<eowu::schema::States> get_render_state_schema(const std::string &file);
  eowu::LuaFunctionContainerType get_render_functions(const eowu::schema::States &schema);
  eowu::LuaFunctionContainerType get_flip_functions(const eowu::schema::States &schema);
  eowu::SetupStatus parse_schemas(const std::string &file);
  eowu::SetupStatus validate_schemas(const eowu::schema::Setup &schema, const std::string &file);
  
  static std::shared_ptr<eowu::LuaContext> get_new_lua_context();
  static lua_State* get_new_lua_state();
  static void initialize_schemas(lua_State *L);
};
