//
//  LuaRuntime.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "LuaRuntime.hpp"
#include <iostream>
#include <eowu-common/logging.hpp>

eowu::LuaRuntime::LuaRuntime(eowu::StateManager &manager,
                             eowu::StateRunner &runner) :
state_manager(manager), state_runner(runner) {
  //
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

eowu::parser::ParseResult<eowu::schema::States> eowu::LuaRuntime::get_render_state_schema(const std::string &file) {
  auto render_parse_result = eowu::parser::entry_script(lua_contexts.render->GetState(), file);
  
  const luabridge::LuaRef &render_entry_table = render_parse_result.result.entry_table;
  const luabridge::LuaRef &states = render_entry_table[eowu::constants::eowu_states_table_name];
  
  return eowu::parser::states(states);
}

eowu::LuaFunctionContainerType eowu::LuaRuntime::get_render_functions(const eowu::schema::States &schema) {
  return init::get_render_functions(schema);
}

eowu::LuaFunctionContainerType eowu::LuaRuntime::get_flip_functions(const eowu::schema::States &schema) {
  return init::get_flip_functions(schema);
}

bool eowu::LuaRuntime::InitializeSchema(const std::string &file) {
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
  
  return true;
}

void eowu::LuaRuntime::InitializeScriptWrapper(eowu::ScriptWrapper &script_wrapper,
                                               const std::string &file,
                                               std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  
  const auto render_state_schema = get_render_state_schema(file);
  //  should already have been validated.
  assert(render_state_schema.success);
  
  auto render_functions = get_render_functions(render_state_schema.result);
  auto flip_functions = get_flip_functions(render_state_schema.result);
  auto states = init::get_states(setup_schema.states, lua_contexts.task, state_manager, state_runner);
  
  auto lua_render_functions = std::make_shared<eowu::LockedLuaRenderFunctions>(nullptr, nullptr);
  
  script_wrapper.SetVariables(setup_schema.variables.mapping);
  script_wrapper.SetStateWrapperContainer(std::move(states));
  script_wrapper.SetGLPipeline(gl_pipeline);
  script_wrapper.SetRenderFunctions(std::move(render_functions));
  script_wrapper.SetFlipFunctions(std::move(flip_functions));
  script_wrapper.SetLuaRenderFunctionPair(lua_render_functions);
}

eowu::State* eowu::LuaRuntime::GetFirstState() {
  return init::get_first_state(setup_schema.states, state_manager);
}

std::shared_ptr<eowu::LuaContext> eowu::LuaRuntime::get_new_lua_context() {
  return std::make_shared<eowu::LuaContext>(get_new_lua_state());
}

void eowu::LuaRuntime::initialize_lua_contexts() {
  lua_contexts.task = get_new_lua_context();
  lua_contexts.render = get_new_lua_context();
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
