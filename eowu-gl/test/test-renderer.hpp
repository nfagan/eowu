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

bool any_should_close(const std::vector<eowu::WindowType> &wins) {
  for (const auto& win : wins) {
    if (win->ShouldClose()) {
      return true;
    }
  }
  return false;
}

void test_renderer_instantiation() {
  
  auto manager = std::make_shared<eowu::ContextManager>();
  auto renderer = eowu::Renderer(manager);
  
  manager->Initialize();
  
  auto win = manager->OpenWindow(640, 480);
  auto win2 = manager->OpenWindow(1000, 480);
  
  win->MakeCurrent();
  
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  
  auto mesh = std::make_shared<eowu::Mesh>();
  auto material = std::make_shared<eowu::Material>();
  auto material2 = std::make_shared<eowu::Material>(material);
  
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
  
  while (!any_should_close(windows)) {
    renderer.Queue({model, model2});
    renderer.Draw({win2, win});
    renderer.ClearQueue();
    
    glfwPollEvents();
  }
  
  glfwTerminate();
}

void test_renderer_run_all() {
  std::cout << "--------" << "renderer" << std::endl;
  test_renderer_instantiation();
}
