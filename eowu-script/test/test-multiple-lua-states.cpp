//
//  test-multiple-lua-states.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#include "test-multiple-lua-states.hpp"
#include <eowu-script/eowu-script.hpp>
#include "test-util.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

namespace test_states {
  void task_thread();
  void render_thread();
  void render_change_handler(lua_State *L);
  void insert_renderer(lua_State *L);
  void render_change_handler_raw(std::string func, lua_State *L);
  void insert_new_render_function(lua_State *L, const std::string &render_source);
  
  std::recursive_mutex render_function_assignment_mutex;
  std::string render_function_source;
  std::atomic<bool> new_render_function_source_available = false;
}

void test_states::insert_new_render_function(lua_State *L, const std::string &render_source) {
  std::string func = "function RenderFunction() print('hello from new func') end";
  
  lua_getglobal(L, "AssignFunction");
//  lua_pushstring(L, func.c_str());
  lua_pushlstring(L, &render_source[0], render_source.length());
  
  int res = lua_pcall(L, 1, 1, 0);
  
  if (res != 0) {
    std::cout << "ERROR: Failed to load function" << std::endl;
    return;
  }
  
  lua_pushvalue(L, -1);
  lua_setglobal(L, "RenderFunction");
  
}

void test_states::render_change_handler_raw(std::string func, lua_State *L) {
  std::unique_lock<std::recursive_mutex> lock(test_states::render_function_assignment_mutex);
  std::cout << "Calling Raw ... " << std::endl;
  
  std::cout << "FUNC IS: " << func << std::endl;
}

void test_states::render_change_handler(lua_State *L) {
  std::unique_lock<std::recursive_mutex> lock(test_states::render_function_assignment_mutex);
  std::cout << "Calling ... " << std::endl;
  
  if (!lua_isfunction(L, -1)) {
    std::cout << "Input is not function" << std::endl;
    return;
  }
  
  //  get reference to string -> dump
  lua_getglobal(L, "string");
  lua_pushstring(L, "dump");
  lua_gettable(L, -2);
  lua_pushvalue(L, -3);
  
  //  string.dump(function() ... end)
  int res = lua_pcall(L, 1, 1, 0);
  
  if (res != 0) {
    std::cout << "ERROR" << std::endl;
    return;
  }
  
  //  make sure output is a string
  int lua_t = lua_type(L, -1);
  
  if (lua_t == LUA_TSTRING) {
    const char *raw_result = lua_tolstring(L, -1, NULL);
    
    std::string result(raw_result);
    
    render_function_source = result;
    
    std::cout << "Loaded source: " << render_function_source << std::endl;
    lua_pop(L, 1);
    
    new_render_function_source_available = true;
  } else {
    std::cout << "ERROR: Function did not return string" << std::endl;
  }
}

void test_states::insert_renderer(lua_State *L) {
  using namespace luabridge;
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .addFunction("Render", &test_states::render_change_handler)
  .addFunction("RenderRaw", &test_states::render_change_handler_raw)
  .endNamespace();
}

//
//  render
//

void test_states::render_thread() {
  using namespace eowu;
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  auto lua_context_manager = std::make_shared<LuaContext>(L);
  
  auto file = util::get_lua_test_script_directory() + "test-multiple-states.lua";
  LuaRef lua_render = util::get_global_from_script_with_trap(L, file, "RenderFunction");
  
  while (true) {
    std::unique_lock<std::recursive_mutex> lock(test_states::render_function_assignment_mutex);
    
    if (new_render_function_source_available) {
      test_states::insert_new_render_function(L, render_function_source);
      new_render_function_source_available = false;
    }
    
    LuaRef lua_render = getGlobal(L, "RenderFunction");
    lua_render();
  }
  
  std::cout << "OK-render" << std::endl;
}

//
//  task
//

void test_states::task_thread() {
  using namespace eowu;
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  test_states::insert_renderer(L);
  
  auto lua_context_manager = std::make_shared<LuaContext>(L);
  
  auto file = util::get_lua_test_script_directory() + "test-multiple-states.lua";
  LuaRef states = util::get_global_from_script_with_trap(L, file, "States");
  
  LuaRef lua_loop = states["Loop"];
  
  while (true) {
    lua_loop();
  }
  
  std::cout << "OK-task" << std::endl;
}

void test_states::test_states_main() {
  auto t1 = std::thread(task_thread);
  
  render_thread();
  
  t1.join();
}
