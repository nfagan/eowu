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
  std::atomic<bool> threads_should_continue = true;
  //  end globals
  
  void frame_summary(std::vector<double> frame_times) {
    double mean = util::mean(frame_times);
    double std = util::std(frame_times);
    double max = util::max(frame_times);
    double min = util::min(frame_times);
    
    std::cout << "single context" << std::endl;
    std::cout << "MEAN: " << mean << std::endl;
    std::cout << "STD: " << std << std::endl;
    std::cout << "MIN: " << min << std::endl;
    std::cout << "MAX: " << max << std::endl;
    std::cout << "N: " << frame_times.size() << std::endl;
  }
  
  void main_create() {
    using namespace eowu;
    
    auto pipeline = GLPipeline::GetInstance();
    assert(pipeline);
    
    auto gl_context_manager = pipeline->GetContextManager();
    auto gl_resource_manager = pipeline->GetResourceManager();
    auto gl_texture_manager = pipeline->GetTextureManager();
    
    gl_context_manager->Initialize();
    auto win = gl_context_manager->OpenWindow(500, 500);
    
    const std::string stim_id = "first";
    
    auto mesh = gl_resource_manager->Create<Mesh>(stim_id);
    auto material = gl_resource_manager->Create<Material>(stim_id);
    auto stim1 = gl_resource_manager->Create<Model>(stim_id, mesh, material);
    
    for (unsigned int i = 1; i < 1001; i++) {
      auto stim = gl_resource_manager->Create<Model>("first" + std::to_string(i), mesh, material);
    }
    
    gl_texture_manager->LoadImage("/Users/Nick/Desktop/eg.png", "first");
    
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
    
    auto t1 = eowu::time::now();
    auto start = t1;
    bool first_iter = true;
    std::vector<double> frame_times;
    
    while (threads_should_continue && !win->ShouldClose()) {
      if (lua_setup_complete) {
        lua_context_manager->Call(*render_function.get());
      }
      
      renderer->Draw(win);
      renderer->ClearQueue();
      context_manager->PollEvents();
      
      auto t2 = eowu::time::now();
      
      std::chrono::duration<double, std::milli> total = t2 - start;
      
      if (total.count() > 10000.0) {
        threads_should_continue = false;
      }
      
      if (!first_iter) {
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        frame_times.push_back(fp_ms.count());
        t1 = t2;
      } else {
        first_iter = false;
        t1 = eowu::time::now();
      }
    }
    
    frame_summary(frame_times);
    
    threads_should_continue = false;
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
      std::cout << "ERROR: Failed to parse schema with message: " << state_schemas.message << std::endl;
      return;
    }
    
    LuaRef render_func = state_schemas.result.mapping.at("State1").entry_function;
    LuaReferenceContainer render_func_ref(render_func);
    render_function = std::make_shared<eowu::LuaFunction>(render_func_ref);
    
    eowu::TaskWrapper::States = init::get_states(state_schemas.result, lua_context_manager, state_manager);
    eowu::GLPipelineWrapper::Pipeline = pipeline;
    eowu::GLPipelineWrapper::LuaRenderFunction = render_function;
    
    init::init_state_schema(lua_context_manager->GetState());
    init::init_render_schema(lua_context_manager->GetState());
    
    lua_setup_complete = true;
    
    runner.Begin(state_manager.GetState("1"));
    
    while (threads_should_continue && !runner.Update()) {
      //
    }
    
    threads_should_continue = false;
  }
  
  void test_gl_wrapper_run_all() {
    main_create();
  }
}
