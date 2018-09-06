//
//  test-script-wrapper.cpp
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

namespace script {
  std::atomic<bool> render_setup_complete = false;
  std::atomic<bool> task_setup_complete = false;
  std::atomic<bool> threads_should_proceed = true;
  
  eowu::schema::States get_state_schema(lua_State *L) {
    auto file = util::get_lua_test_script_directory() + "test-alternate-threaded-model.lua";
    luabridge::LuaRef state = util::get_global_from_script_with_trap(L, file, "States");
    auto parse_result = eowu::parser::states(state);
    assert(parse_result.success);
    return parse_result.result;
  }
  
  std::shared_ptr<eowu::LuaContext> get_lua_context() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    return std::make_shared<eowu::LuaContext>(L);
  }
  
  void init_schemas(std::shared_ptr<eowu::LuaContext> lua_context) {
    eowu::init::init_state_schema(lua_context->GetState());
    eowu::init::init_render_schema(lua_context->GetState());
    eowu::ScriptWrapper::CreateLuaSchema(lua_context->GetState());
  }
  
  void frame_summary(std::vector<double> frame_times) {
    double mean = util::mean(frame_times);
    double std = util::std(frame_times);
    double max = util::max(frame_times);
    double min = util::min(frame_times);
    
    std::cout << "multiple context" << std::endl;
    std::cout << "MEAN: " << mean << std::endl;
    std::cout << "STD: " << std << std::endl;
    std::cout << "MIN: " << min << std::endl;
    std::cout << "MAX: " << max << std::endl;
    std::cout << "N: " << frame_times.size() << std::endl;
  }
  
  //
  //  task thread
  //
  
  void task_thread(eowu::ScriptWrapper &wrapper, std::shared_ptr<eowu::GLPipeline> pipeline) {
    using namespace eowu;
    auto lua_context = get_lua_context();
    
    StateManager state_manager;
    StateRunner state_runner;
    
    auto schema = get_state_schema(lua_context->GetState());
    auto states = init::get_states(schema, lua_context, state_manager);
    
    init_schemas(lua_context);
    
    wrapper.SetGLPipeline(pipeline);
    wrapper.SetStateWrapperContainer(std::move(states));
    
    auto first_state = state_manager.GetState("1");
    state_runner.Begin(first_state);
    
    task_setup_complete = true;
    
    while (!render_setup_complete) {
      //
    }
    
    while (threads_should_proceed && !state_runner.Update()) {
      //
    }
    
    threads_should_proceed = false;
  }
  
  //
  //  render thread
  //
  
  void render_thread(eowu::ScriptWrapper &wrapper,
                     std::shared_ptr<eowu::GLPipeline> pipeline,
                     eowu::WindowType win) {
    
    using namespace eowu;
    
    auto lua_context = get_lua_context();
    auto schema = get_state_schema(lua_context->GetState());
    
    init_schemas(lua_context);
    
    auto render_functions = init::get_render_functions(schema);
    ScriptWrapper::LuaRenderFunction = std::make_shared<LuaFunction>(render_functions->at("state1"));
    
    wrapper.SetRenderFunctions(std::move(render_functions));
    
    render_setup_complete = true;
    
    auto renderer = pipeline->GetRenderer();
    auto context = pipeline->GetContextManager();
    
    while (!task_setup_complete) {
      //
    }
    
    auto t1 = eowu::time::now();
    auto start = t1;
    bool first_iter = true;
    std::vector<double> frame_times;
    
    while (threads_should_proceed && !win->ShouldClose()) {
      lua_context->Call(*ScriptWrapper::LuaRenderFunction.get());
      renderer->Draw(win);
      renderer->ClearQueue();
      context->PollEvents();
      
      auto t2 = eowu::time::now();
      
      std::chrono::duration<double, std::milli> total = t2 - start;
      
      if (total.count() > 10000.0) {
        threads_should_proceed = false;
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
    
    win->Close();
    
    threads_should_proceed = false;
  }
  
  void insert_gl_resources(std::shared_ptr<eowu::GLPipeline> pipeline) {
    using namespace eowu;
    
    auto gl_resource_manager = pipeline->GetResourceManager();
    auto gl_texture_manager = pipeline->GetTextureManager();
    
    const std::string stim_id = "first";
    
    auto mesh = gl_resource_manager->Create<Mesh>(stim_id);
    auto material = gl_resource_manager->Create<Material>(stim_id);
    
    auto stim1 = gl_resource_manager->Create<Model>(stim_id, mesh, material);
    
    for (unsigned int i = 1; i < 1001; i++) {
      auto mat = gl_resource_manager->Create<Material>(stim_id);
      auto stim = gl_resource_manager->Create<Model>("first" + std::to_string(i), mesh, mat);
    }
    
    gl_texture_manager->LoadImage("/Users/Nick/Desktop/eg.png", "first");
    
    material->SetFaceColor(glm::vec3(1.0f));
    
    stim1->GetTransform().SetUnits(units::normalized);
    stim1->GetTransform().SetPosition(glm::vec3(0.5, 0.5, 1.0));
    stim1->GetTransform().SetScale(glm::vec3(0.5, 0.5, 1.0));
    
    mesh_factory::make_quad(mesh.get());
  }
  
  void main_create() {
    using namespace eowu;
    
    auto pipeline = GLPipeline::GetInstance();
    eowu::ScriptWrapper wrapper;
    auto gl_context_manager = pipeline->GetContextManager();
    
    gl_context_manager->Initialize();
    auto win = gl_context_manager->OpenWindow(500, 500);
    
    insert_gl_resources(pipeline);
    
    auto t1 = std::thread(task_thread, std::ref(wrapper), pipeline);
    render_thread(std::ref(wrapper), pipeline, win);
    
    t1.join();
  }
  
  void test_gl_wrapper_run_all() {
    script::main_create();
  }
}

