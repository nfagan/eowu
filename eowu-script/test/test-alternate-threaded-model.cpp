//
//  test-alternate-threaded-model.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/4/18.
//

#include "test-alternate-threaded-model.hpp"
#include "test-util.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <assert.h>
#include <thread>
#include <mutex>
#include <atomic>

namespace testalt {
  void task_thread();
  void render_thread();
  void render_function_callback(const std::string &func);
  void insert_function_callback(lua_State *L);
  eowu::schema::State get_state_schema(lua_State *L, const std::string &file);
  
  //  globals
  std::recursive_mutex render_function_mut;
  std::atomic<bool> render_functions_initialized = false;
  eowu::schema::RenderFunctionAggregateType *render_functions = nullptr;
  luabridge::LuaRef *render_function = nullptr;
  //  end globals
}

void testalt::insert_function_callback(lua_State *L) {
  using namespace luabridge;
  
  getGlobalNamespace(L)
  .beginNamespace("test")
  .addFunction("first", &render_function_callback)
  .endNamespace();
}

void testalt::render_function_callback(const std::string &func) {
  std::unique_lock<std::recursive_mutex> lock(render_function_mut);
  
  assert(render_functions);
  render_function = &render_functions->at(func);
}

eowu::schema::State testalt::get_state_schema(lua_State *L, const std::string &file) {
  luabridge::LuaRef state = util::get_global_from_script_with_trap(L, file, "State");
  auto parse_result = eowu::parser::state(state);
  assert(parse_result.success);
  return parse_result.result;
}

void testalt::main_create() {
  using namespace eowu;
  
  auto t1 = std::thread(task_thread);
  render_thread();
  
  t1.join();
}

void testalt::render_thread() {
  using namespace eowu;
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  auto file = util::get_lua_test_script_directory() + "test-alternate-threaded-model.lua";
  auto schema = testalt::get_state_schema(L, file);
  
  testalt::insert_function_callback(L);
  
  render_functions = &schema.render_functions;
  render_function = &schema.render_functions.at("default");
  
  render_functions_initialized = true;
  
  while (true) {
    std::unique_lock<std::recursive_mutex> lock(render_function_mut);
    (*render_function)();
  }
}

void testalt::task_thread() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  auto file = util::get_lua_test_script_directory() + "test-alternate-threaded-model.lua";
  auto schema = testalt::get_state_schema(L, file);
  
  testalt::insert_function_callback(L);
  
  auto loop_func = schema.loop_function;
  
  while (!render_functions_initialized) {
    //
  }
  
  while (true) {
    loop_func();
  }
}

void testalt::test_alternate_run_all() {
  testalt::main_create();
}
