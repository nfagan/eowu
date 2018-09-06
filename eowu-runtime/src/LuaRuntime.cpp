//
//  LuaRuntime.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "LuaRuntime.hpp"
#include <iostream>

eowu::LuaRuntime::LuaRuntime() {
  is_initialized = false;
}

eowu::SetupStatus eowu::LuaRuntime::parse_schemas(const std::string &file) {
  eowu::SetupStatus result;
  
  auto entry_parse_result = eowu::parser::entry_script(lua_contexts.task->GetState(), file);
  
  //  file is invalid
  if (!entry_parse_result.success) {
    return SetupStatus::entry_file_error(file, entry_parse_result.message);
  }
  
  const luabridge::LuaRef &entry_table = entry_parse_result.result.entry_table;
  
  auto setup_parse_result = eowu::parser::setup(entry_table);
  
  //  setup is invalid
  if (!setup_parse_result.success) {
    return SetupStatus::entry_parse_error(file, setup_parse_result.message, setup_parse_result.context);
  }
  
  setup_schema = setup_parse_result.result;
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::LuaRuntime::validate_schemas(const eowu::schema::Setup &schema,
                                                     const std::string &file) {
  eowu::SetupStatus result;
  
  auto setup_validator = eowu::validate::make_setup(schema);
  auto validation_result = eowu::validate::setup(schema, setup_validator);
  
  if (!validation_result.success) {
    return SetupStatus::entry_file_validation_error(file, validation_result.message, validation_result.context);
  }
  
  result.success = true;
  
  return result;
}

eowu::RenderFunctionContainerType eowu::LuaRuntime::get_render_functions(const std::string &file) {
  auto render_parse_result = eowu::parser::entry_script(lua_contexts.render->GetState(), file);
  
  const luabridge::LuaRef &render_entry_table = render_parse_result.result.entry_table;
  const luabridge::LuaRef &states = render_entry_table["States"];
  
  const auto render_state_schema = eowu::parser::states(states);
  
  assert(render_state_schema.success);
  
  return init::get_render_functions(render_state_schema.result);
}

bool eowu::LuaRuntime::Initialize(const std::string &file) {
  initialize_lua_contexts();
  
  auto parse_result = parse_schemas(file);
  
  if (!parse_result.success) {
    parse_result.print();
    return false;
  }
  
  auto validation_result = validate_schemas(setup_schema, file);
  
  if (!validation_result.success) {
    validation_result.print();
    return false;
  }
  
  //
  //  insert schema into lua state
  //
  
  initialize_schemas(lua_contexts.task->GetState());
  initialize_schemas(lua_contexts.render->GetState());
  
  //
  //  extract render functions
  //
  
  render_functions = get_render_functions(file);
  
  initialize_state_functions();
  initialize_render_functions();
  
  return true;
}

void eowu::LuaRuntime::initialize_state_functions() {
  auto states = init::get_states(setup_schema.states, lua_contexts.task, state_manager);
  script_wrapper.SetStateWrapperContainer(std::move(states));
}

void eowu::LuaRuntime::initialize_render_functions() {
  eowu::ScriptWrapper::LuaRenderFunction = std::make_shared<LuaFunction>(render_functions->at("hello"));
  script_wrapper.SetRenderFunctions(std::move(render_functions));
}

void eowu::LuaRuntime::initialize_lua_contexts() {
  lua_contexts.task = std::make_shared<eowu::LuaContext>(get_new_lua_state());
  lua_contexts.render = std::make_shared<eowu::LuaContext>(get_new_lua_state());
}

lua_State* eowu::LuaRuntime::get_new_lua_state() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  return L;
}

void eowu::LuaRuntime::initialize_schemas(lua_State *L) {
  eowu::init::init_state_schema(L);
  eowu::init::init_render_schema(L);
  eowu::ScriptWrapper::CreateLuaSchema(L);
}
