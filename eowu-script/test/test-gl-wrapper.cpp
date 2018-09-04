//
//  test-gl-wrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "test-gl-wrapper.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-state/eowu-state.hpp>
#include "test-util.hpp"
#include <assert.h>
#include <iostream>
#include <string>
#include <thread>
#include <memory>
#include <atomic>

namespace gltest {
  void task_thread(std::shared_ptr<eowu::GLPipeline> pipeline);
  void render_thread(std::shared_ptr<eowu::GLPipeline> pipeline, eowu::WindowType win);
  
  //  globals
  std::shared_ptr<eowu::LuaFunction> render_function = nullptr;
  std::shared_ptr<eowu::LuaContext> lua_context_manager = nullptr;
  std::atomic<bool> lua_setup_complete = false;
  //  end globals
  
  void main_create() {
    using namespace eowu;
    
    auto pipeline = GLPipeline::GetInstance();
    assert(pipeline);
    
    auto gl_context_manager = pipeline->GetContextManager();
    auto gl_resource_manager = pipeline->GetResourceManager();
    
    gl_context_manager->Initialize();
    auto win = gl_context_manager->OpenWindow();
    
    const std::string stim_id = "first";
    
    auto mesh = gl_resource_manager->Create<Mesh>(stim_id);
    auto material = gl_resource_manager->Create<Material>(stim_id);
    auto stim1 = gl_resource_manager->Create<Model>(stim_id, mesh, material);
    
    material->SetFaceColor(glm::vec3(1.0f));
    
    stim1->GetTransform().SetUnits(units::normalized);
    stim1->GetTransform().SetPosition(glm::vec3(0.5, 0.5, 1.0));
    stim1->GetTransform().SetScale(glm::vec3(0.5, 0.5, 1.0));
    
    mesh_factory::make_quad(mesh.get());
    
    auto t1 = std::thread(task_thread, pipeline);
    render_thread(pipeline, win);
    
    t1.join();
  }
  
  void render_thread(std::shared_ptr<eowu::GLPipeline> pipeline, eowu::WindowType win) {
    
    auto renderer = pipeline->GetRenderer();
    auto context_manager = pipeline->GetContextManager();
    
    while (true) {
      if (lua_setup_complete) {
        lua_context_manager->Call(*render_function.get());
      }
      
      renderer->Draw(win);
      renderer->ClearQueue();
      
      context_manager->PollEvents();
    }
  }
  
  void task_thread(std::shared_ptr<eowu::GLPipeline> pipeline) {
    using namespace eowu;
    using namespace luabridge;
    
    StateManager state_manager;
    StateRunner runner;
    
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    lua_context_manager = std::make_shared<LuaContext>(L);
    
    auto file = util::get_lua_test_script_directory() + "test-state-wrapper.lua";
    LuaRef states = util::get_global_from_script_with_trap(L, file, "States");
    
    auto state_schemas = parser::states(states);
    
    if (!state_schemas.success) {
      std::cout << "ERROR: Failed to parse schema." << std::endl;
      return;
    }
    
    LuaRef render_func = state_schemas.result.mapping.at("State1").render_function;
    LuaReferenceContainer render_func_ref(render_func);
    render_function = std::make_shared<eowu::LuaFunction>(render_func_ref);
    
    init::init_states(lua_context_manager, state_manager, state_schemas.result);
    init::init_render_pipeline(lua_context_manager, render_function, pipeline);
    
    lua_setup_complete = true;
    
    runner.Next(state_manager.GetState("1"));
    
    while (!runner.Update()) {
      //
    }
    
    
  }
  
  void test_gl_wrapper_run_all() {
    main_create();
  }
}
