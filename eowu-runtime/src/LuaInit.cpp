//
//  LuaInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/22/18.
//

#include "LuaInit.hpp"
#include "Error.hpp"
#include "Lua.hpp"
#include <eowu-state/eowu-state.hpp>

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

eowu::init::ContextResult eowu::init::initialize_lua_contexts() {
  eowu::init::ContextResult result;
  
  auto task_context = create_lua_context();
  auto render_context = create_lua_context();
  
  insert_lua_callbacks(task_context->GetState());
  insert_lua_callbacks(render_context->GetState());
  
  result.result.task = task_context;
  result.result.render = render_context;
  
  result.status.success = true;
  
  return result;
}

eowu::init::SchemaResult eowu::init::initialize_schema(const std::string &file,
                                                       std::shared_ptr<eowu::LuaContext> task_context) {
  
  eowu::init::SchemaResult result;
  
  auto *L = task_context->GetState();
  
  try {
    auto setup_schema = parse_setup_schema(file, L);
    
    validate_setup_schema(setup_schema);
    
    result.result = std::move(setup_schema);
  } catch (const eowu::SetupError &e) {
    
    result.status.message = e.what();
    result.status.context = e.context;
    result.status.file = file;
    
    return result;
  }
  
  result.status.success = true;
  
  return result;
}

eowu::init::ScriptWrapperResult eowu::init::initialize_script_wrapper(const std::string &file,
                                                                      const eowu::schema::Setup &schema,
                                                                      std::shared_ptr<eowu::GLPipeline> gl_pipeline,
                                                                      eowu::init::LuaContexts lua_contexts,
                                                                      eowu::StateManager &state_manager,
                                                                      eowu::StateRunner &state_runner) {
  eowu::init::ScriptWrapperResult result;
  
  const auto render_state_schema = get_render_state_schema(file, lua_contexts.render->GetState());
  //  should already have been validated.
  assert(render_state_schema.success);
  
  auto render_functions = init::get_render_functions(render_state_schema.result);
  auto flip_functions = init::get_flip_functions(render_state_schema.result);
  auto states = init::get_states(schema.states, lua_contexts.task, state_manager, state_runner);
  
  auto lua_render_functions = std::make_shared<eowu::LockedLuaRenderFunctions>(nullptr, nullptr);
  auto keyboard_wrapper = create_keyboard(gl_pipeline->GetContextManager()->GetKeyboard());
  
  eowu::ScriptWrapper::SetVariables(schema.variables.mapping);
  eowu::ScriptWrapper::SetStateWrapperContainer(std::move(states));
  eowu::ScriptWrapper::SetGLPipeline(gl_pipeline);
  eowu::ScriptWrapper::SetRenderFunctions(std::move(render_functions));
  eowu::ScriptWrapper::SetFlipFunctions(std::move(flip_functions));
  eowu::ScriptWrapper::SetLuaRenderFunctionPair(lua_render_functions);
  eowu::ScriptWrapper::SetKeyboardWrapper(std::move(keyboard_wrapper));
  eowu::ScriptWrapper::SetLuaTaskContext(lua_contexts.task);
  eowu::ScriptWrapper::SetLuaRenderContext(lua_contexts.render);
  eowu::ScriptWrapper::SetStateRunner(&state_runner);
  
  result.result.first_state = try_get_first_state(schema.states, state_manager);
  
  result.status.success = true;
  
  return result;
}

eowu::schema::Setup eowu::init::parse_setup_schema(const std::string &file,
                                                   lua_State *lua_task_state) {
  
  auto entry_parse_result = eowu::parser::entry_script(lua_task_state, file);
  
  //  file is invalid
  if (!entry_parse_result.success) {
    throw eowu::SetupError(entry_parse_result.message, entry_parse_result.context);
  }
  
  const luabridge::LuaRef &entry_table = entry_parse_result.result.entry_table;
  
  auto setup_parse_result = eowu::parser::setup(entry_table);
  
  //  setup is invalid
  if (!setup_parse_result.success) {
    throw eowu::SetupError(setup_parse_result.message, setup_parse_result.context);
  }
  
  return setup_parse_result.result;
}

void eowu::init::validate_setup_schema(const eowu::schema::Setup &schema) {
  
  auto setup_validator = eowu::validate::make_setup(schema);
  auto validation_result = eowu::validate::setup(schema, setup_validator);
  
  if (!validation_result.success) {
    throw eowu::SetupError(validation_result.message, validation_result.context);
  }
}

void eowu::init::insert_lua_callbacks(lua_State *L) {
  eowu::init::init_static_schemas(L);
  eowu::init::init_state_schema(L);
  eowu::init::init_render_schema(L);
  eowu::init::init_audio_schemas(L);
  eowu::ScriptWrapper::CreateLuaSchema(L);
}

std::shared_ptr<eowu::LuaContext> eowu::init::create_lua_context() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  return std::make_shared<eowu::LuaContext>(L);
}

std::unique_ptr<eowu::KeyboardWrapper> eowu::init::create_keyboard(eowu::Keyboard& keyboard) {
  return std::make_unique<eowu::KeyboardWrapper>(keyboard);
}

eowu::parser::ParseResult<eowu::schema::States> eowu::init::get_render_state_schema(const std::string &file,
                                                                                    lua_State *render_state) {
  auto render_parse_result = eowu::parser::entry_script(render_state, file);
  
  const luabridge::LuaRef &render_entry_table = render_parse_result.result.entry_table;
  const luabridge::LuaRef &states = render_entry_table[eowu::constants::eowu_states_table_name];
  
  return eowu::parser::states(states);
}

eowu::State* eowu::init::try_get_first_state(const eowu::schema::States &schema,
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
    auto wrapper = std::make_shared<eowu::StateWrapper>(state,
                                                        schema.variables,
                                                        lua_context,
                                                        std::move(state_functions));
    
    result->emplace(schema.state_id, std::move(wrapper));
  }
  
  return result;
}

void eowu::init::init_static_schemas(lua_State *L) {
  eowu::FilesystemWrapper::CreateLuaSchema(L);
  eowu::time::wrapper::CreateLuaSchema(L);
}

void eowu::init::init_render_schema(lua_State *L) {
  eowu::RendererWrapper::CreateLuaSchema(L);
  eowu::VectorWrapper::CreateLuaSchema(L);
  eowu::ModelWrapper::CreateLuaSchema(L);
  eowu::TextModelWrapper::CreateLuaSchema(L);
  eowu::KeyboardWrapper::CreateLuaSchema(L);
  eowu::WindowWrapper::CreateLuaSchema(L);
}

void eowu::init::init_state_schema(lua_State *L) {
  eowu::StateWrapper::CreateLuaSchema(L);
  eowu::VariableWrapper::CreateLuaSchema(L);
  eowu::VariablesWrapper::CreateLuaSchema(L);
  eowu::MathWrapper::CreateLuaSchema(L);
  eowu::TargetWrapper::CreateLuaSchema(L);
  eowu::TargetSetWrapper::CreateLuaSchema(L);
  eowu::TimeoutWrapper::CreateLuaSchema(L);
  eowu::TimeoutHandleWrapper::CreateLuaSchema(L);
  eowu::XYSourceWrapper::CreateLuaSchema(L);
}

void eowu::init::init_audio_schemas(lua_State *L) {
  eowu::AudioSourceHandleWrapper::CreateLuaSchema(L);
  eowu::AudioSourceWrapper::CreateLuaSchema(L);
}

EOWU_GET_LUA_FUNCTION_CONTAINER(get_render_functions, render_functions)
EOWU_GET_LUA_FUNCTION_CONTAINER(get_flip_functions, flip_functions)
