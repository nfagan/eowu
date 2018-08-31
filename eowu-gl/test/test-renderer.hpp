//
//  test-renderer.hpp
//  eowu
//
//  Created by Nick Fagan on 8/30/18.
//

#pragma once

#include <eowu-gl/eowu-gl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <assert.h>
#include <chrono>

bool any_should_close(const std::vector<eowu::WindowType> &wins) {
  for (const auto& win : wins) {
    if (win->ShouldClose()) {
      return true;
    }
  }
  return false;
}

void test_many_models() {
  auto manager = std::make_shared<eowu::ContextManager>();
  auto renderer = eowu::Renderer(manager);
  
  manager->Initialize();
  
  auto win = manager->OpenWindow();
  
  win->MakeCurrent();
  
  auto mesh = std::make_shared<eowu::Mesh>();
  auto material = std::make_shared<eowu::Material>();
  
  eowu::mesh_factory::make_quad(mesh.get());
  
  std::vector<eowu::Model> models;
  
  eowu::u32 n_models = 1000;
  
  for (eowu::u32 i = 0; i < n_models; i++) {
    eowu::Model model(mesh, material);
    auto &transform = model.GetTransform();
    
    transform.SetUnits(eowu::units::normalized);
    transform.SetScreenDimensions(win->GetDimensions());
    transform.SetScale(glm::vec3(0.005f));
    
    models.push_back(model);
  }
  
  material->SetFaceColor(glm::vec3(1.0f));
  
  while (!win->ShouldClose()) {
    
    for (auto& model : models) {
      auto &transform = model.GetTransform();
      
      float x = (float)rand() / (float)RAND_MAX;
      float y = (float)rand() / (float)RAND_MAX;
      
      transform.SetPosition(glm::vec3(x, y, 1.0f));
    }
    
    renderer.Queue(models);
    renderer.Draw(win);
    renderer.ClearQueue();
    glfwPollEvents();
  }
  
  glfwTerminate();
}

void test_renderer_instantiation() {
  
  auto manager = std::make_shared<eowu::ContextManager>();
  auto renderer = eowu::Renderer(manager);
  
  manager->Initialize();
  
  auto win = manager->OpenWindow(640, 480);
  auto win2 = manager->OpenWindow(1000, 480);
  
  win->MakeCurrent();
  
  auto mesh = std::make_shared<eowu::Mesh>();
  auto material = std::make_shared<eowu::Material>();
  auto material2 = std::make_shared<eowu::Material>(material);
  auto tex = eowu::load::image("/Users/Nick/Documents/MATLAB/repositories/hwwba/stimuli/ac/go1.png");
  
  material2->SetFaceColor(tex);
  
  eowu::mesh_factory::make_quad(mesh.get());
  eowu::Model model(mesh, material);
  eowu::Model model2(mesh, material2);
  
  eowu::Transform& transform = model.GetTransform();
  transform.SetUnits(eowu::units::normalized);
  transform.SetScreenDimensions(glm::vec2(win->GetWidth(), win->GetHeight()));
  transform.SetPosition(glm::vec3(0.5f, 0.5f, 1.0f));
  transform.SetScale(glm::vec3(0.5f));
  
  model2.SetTransform(model.GetTransform());
  model2.GetTransform().SetScale(glm::vec3(0.25f));
  
  material->SetFaceColor(glm::vec3(0.25f, 1.0f, 0.25f));
  
  std::vector<decltype(win)> windows = {win, win2};
  
  auto starting_time = std::chrono::high_resolution_clock::now();
  bool did_set = false;
  bool did_set2 = false;
  
  while (!any_should_close(windows)) {
    renderer.Queue({model, model2});
    renderer.Draw(windows);
    renderer.ClearQueue();
    
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff_time = current_time - starting_time;
    
    if (!did_set && diff_time.count() > 5000.0) {
      material->SetFaceColor(tex);
      did_set = true;
    }
    
    if (!did_set2 && diff_time.count() > 10000.0) {
      material->SetFaceColor(glm::vec3(0.5f));
      did_set2 = true;
    }
    
    glfwPollEvents();
  }
  
  glfwTerminate();
}

void test_renderer_run_all() {
  std::cout << "--------" << "renderer" << std::endl;
  test_renderer_instantiation();
  test_many_models();
}
