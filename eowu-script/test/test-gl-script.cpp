//
//  test-gl-script.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-gl-script.hpp"
#include "test-util.hpp"
#include <Lua.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <iostream>
#include <thread>
#include <mutex>

namespace gl {

void task_thread(std::shared_ptr<eowu::RenderFunction> render_func);
void render_thread(std::shared_ptr<eowu::Renderer> renderer,
                   std::shared_ptr<eowu::Window> window,
                   std::shared_ptr<eowu::RenderFunction> render_func);

static std::shared_ptr<eowu::RenderFunction> render_func;

std::mutex lua_context_mutex;
std::mutex lua_call_mutex;
luabridge::LuaRef *lua_render_function = nullptr;

void main_create() {
  using namespace eowu;
  
  auto context = std::make_shared<ContextManager>();
  auto resource = std::make_shared<ResourceManager>();
  auto renderer = std::make_shared<Renderer>(context);
  render_func = std::make_shared<RenderFunction>();
  
  context->Initialize();
  
  auto mesh = resource->Create<Mesh>("first");
  auto material = resource->Create<Material>("first");
  auto model = resource->Create<Model>("first", mesh, material);
  
  auto win = context->OpenWindow(400, 400);
  
  model->GetTransform().SetUnits(units::normalized);
  model->GetTransform().SetScreenDimensions(glm::vec2(400, 400));
  model->GetTransform().SetPosition(glm::vec3(0.5, 0.5, 1.0));
  
  auto t1 = std::thread(task_thread, render_func);
  render_thread(renderer, win, render_func);
  
  t1.join();
}

void render_loop_cb_alternate(lua_State *L) {
  auto func = luabridge::LuaRef::fromStack(L, -1);
  
  if (!func.isFunction()) {
    throw std::runtime_error("was not function");
  }
  
  std::cout << "Setting 2... " << std::endl;
  
  std::unique_lock<std::mutex> lk1(lua_context_mutex, std::defer_lock);
  std::unique_lock<std::mutex> lk2(lua_call_mutex, std::defer_lock);
  std::cout << "Locking ... " << std::endl;
  std::lock(lk1, lk2);
  std::cout << "Done locking ... " << std::endl;
  
  *lua_render_function = func;
  
  std::cout << "done" << std::endl;
}

void render_loop_cb(lua_State *L) {
  auto func = luabridge::LuaRef::fromStack(L, -1);
  
  if (!func.isFunction()) {
    throw std::runtime_error("was not function");
  }
  
  std::cout << "Setting ... " << std::endl;
  
  render_func->SetOnNextFrame([&, func]() {
    std::unique_lock<std::mutex> lock(lua_context_mutex);
    func();
  });
}

void task_thread(std::shared_ptr<eowu::RenderFunction> render_func) {
  using namespace eowu;
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .addFunction("test", render_loop_cb_alternate)
  .endNamespace();
  
  auto file = util::get_lua_test_script_directory() + "test-parser-state.lua";
  auto res = util::get_global_from_script_with_trap(L, file, "State1");
  
  if (res.isNil()) {
    std::cout << "No state1 found" << std::endl;
  }
  
  LuaRef lua_loop = res["Loop"];
  LuaRef lua_entry = res["Entry"];
  LuaRef lua_render = res["Render"];
  
  if (!lua_loop.isFunction() || !lua_entry.isFunction() || !lua_render.isFunction()) {
    std::cout << "No state1 loop function found" << std::endl;
  }
  
  lua_render_function = &lua_render;
  
  StateRunner runner;
  StateManager manager;
  
  auto first = manager.CreateState("first");
  first->SetDuration(std::chrono::milliseconds(1));
  runner.Next(first);
  
  first->SetOnLoop([&](auto *state) {
//    std::unique_lock<std::mutex> lock(lua_context_mutex);
    std::unique_lock<std::mutex> lk1(lua_context_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lk2(lua_call_mutex, std::defer_lock);
    std::lock(lk1, lk2);
    
    lua_loop();
  });
  
  first->SetOnEntry([&](auto *state) {
//    std::unique_lock<std::mutex> lock(lua_context_mutex);
    std::unique_lock<std::mutex> lk1(lua_context_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lk2(lua_call_mutex, std::defer_lock);
    std::lock(lk1, lk2);
    lua_entry();
  });
  
  first->SetOnExit([&](auto *state) {
    std::cout << "Exiting" << std::endl;
    std::cout << state->GetTimer().Ellapsed().count() << std::endl;
    state->Next(first);
  });
  
//  render_func->SetOnNextFrame([&](){
//    std::unique_lock<std::mutex> lock(lua_context_mutex);
//    lua_render();
//  });
  
  while (!runner.Update()) {
    //
  }
  
  
}

void render_thread(std::shared_ptr<eowu::Renderer> renderer,
                   std::shared_ptr<eowu::Window> window,
                   std::shared_ptr<eowu::RenderFunction> render_func) {
  while (true) {
    
    {
      std::unique_lock<std::mutex> lk1(lua_context_mutex, std::defer_lock);
      std::unique_lock<std::mutex> lk2(lua_call_mutex, std::defer_lock);
      std::lock(lk1, lk2);
      
      if (lua_render_function != nullptr) {
        (*lua_render_function)();
      }
    }
    
    renderer->Draw(window);
    renderer->ClearQueue();
  }
  
}

void test_gl_script_run_all() {
  main_create();
}
  
}
