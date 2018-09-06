//
//  test-mutex-script.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "test-mutex-script.hpp"
#include "test-util.hpp"
#include <Lua.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-script/eowu-script.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

//
//
//

#define EOWU_IS_WRAPPED

using lock_type = std::unique_lock<std::mutex>;
using rlock_type = std::unique_lock<std::recursive_mutex>;

void task_thread();
void render_thread();
void update_lua_render_cb();

std::mutex lua_context_mutex;
std::mutex lua_function_mutex;

struct lua_function_type {
  lua_function_type(lua_State *L) : fnc(L) {}
  
  void call() const {
    rlock_type lock(mut);
    
    if (fnc.isFunction()) {
      fnc();
    }
  }
  
  void set(const luabridge::LuaRef &fnc) {
    rlock_type lock(mut);
    this->fnc = fnc;
  }
  
  luabridge::LuaRef fnc;
  mutable std::recursive_mutex mut;
};

lua_function_type *lua_render_function = nullptr;

//  WRAPPED
std::shared_ptr<eowu::LuaFunction> lua_render_function_class = nullptr;
std::shared_ptr<eowu::LuaContext> lua_context = nullptr;

//
//
//

void render_loop_cb_alternate(lua_State *L) {
  luabridge::LuaRef func = luabridge::LuaRef::fromStack(L, -1);
  
  if (!func.isFunction()) {
    throw std::runtime_error("was not function");
  }
  
#ifdef EOWU_IS_WRAPPED
  std::cout << "WRAPPED CALLING" << std::endl;
  eowu::LuaReferenceContainer ref(func);
  lua_render_function_class->Set(ref);
#else
  lua_render_function->set(func);
#endif
}

void main_create2() {
  auto t1 = std::thread(task_thread);
  render_thread();
  
  t1.join();
}

void task_thread() {
  using namespace eowu;
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .addFunction("test", render_loop_cb_alternate)
  .endNamespace();
  
  auto file = util::get_lua_test_script_directory() + "test-mutex.lua";
  auto res = util::get_global_from_script_with_trap(L, file, "State");
  
  LuaRef lua_loop = res["Loop"];
  LuaRef lua_entry = res["Entry"];
  LuaRef lua_render = res["Render"];
  
  //  manual
  lua_render_function = new lua_function_type(L);
  lua_render_function->set(lua_render);
  
  //  wrapped
  eowu::LuaReferenceContainer loop_ref(lua_loop);
  eowu::LuaReferenceContainer entry_ref(lua_entry);
  eowu::LuaReferenceContainer render_ref(lua_render);
  
  lua_context = std::make_shared<eowu::LuaContext>(L);
  eowu::LuaFunction lua_loop_wrapped(loop_ref);
  eowu::LuaFunction lua_entry_wrapped(entry_ref);
  lua_render_function_class = std::make_shared<eowu::LuaFunction>(render_ref);
  
  StateRunner runner;
  StateManager manager;
  
  auto first = manager.CreateState("first");
  first->SetDuration(std::chrono::milliseconds(1));
  runner.Begin(first);
  
#ifdef EOWU_IS_WRAPPED
  //  auto
  first->SetOnLoop([&](auto *state) {
    lua_context->Call(lua_loop_wrapped);
  });
  first->SetOnEntry([&](auto *state) {
    lua_context->Call(lua_entry_wrapped);
  });
  first->SetOnExit([&](auto *state) {
    state->Next(first);
  });
#else
  //
  //  manual
  //
  first->SetOnLoop([&](auto *state) {
    lock_type lock(lua_context_mutex);
    lua_loop();
  });
  first->SetOnEntry([&](auto *state) {
    lock_type lock(lua_context_mutex);
    lua_entry();
  });
  first->SetOnExit([&](auto *state) {
    state->Next(first);
  });
#endif
  
  while (!runner.Update()) {
    //
  }
  
  
}

void render_thread() {
  
  while (true) {
#ifdef EOWU_IS_WRAPPED
    if (lua_render_function_class != nullptr) {
      lua_context->Call(*lua_render_function_class.get());
    }
#else
    lock_type lock(lua_context_mutex);
    
    if (lua_render_function != nullptr) {
      lua_render_function->call();
    }
#endif
  }
  
}

void test_mutex_script() {
  main_create2();
}
