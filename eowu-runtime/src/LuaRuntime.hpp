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
  class StateManager;
}

class eowu::LuaRuntime {
  
public:
  LuaRuntime();
  ~LuaRuntime() = default;
  
  bool Initialize(const std::string &file);
  
  eowu::ScriptWrapper script_wrapper;
  eowu::StateManager state_manager;
  eowu::schema::Setup setup_schema;
  eowu::RenderFunctionContainerType render_functions;
  
  struct LuaContexts {
    std::shared_ptr<eowu::LuaContext> task = nullptr;
    std::shared_ptr<eowu::LuaContext> render = nullptr;
  } lua_contexts;
  
private:
  bool is_initialized;
  
  void initialize_lua_contexts();
  void initialize_state_functions();
  void initialize_render_functions();
  
  eowu::RenderFunctionContainerType get_render_functions(const std::string &file);
  eowu::SetupStatus parse_schemas(const std::string &file);
  eowu::SetupStatus validate_schemas(const eowu::schema::Setup &schema, const std::string &file);
  
  static lua_State* get_new_lua_state();
  static void initialize_schemas(lua_State *L);
};
