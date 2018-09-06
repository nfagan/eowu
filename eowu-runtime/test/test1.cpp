//
//  test1.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test1.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-state/eowu-state.hpp>
#include <eowu-script/eowu-script.hpp>
#include <iostream>

void test1() {
  
  using namespace eowu;
  
  auto context_manager = std::make_shared<ContextManager>();
  ResourceManager resource_manager;
  Renderer renderer(context_manager);
  
  //  gl initialization
  context_manager->Initialize();
  
  auto win = context_manager->OpenWindow(400, 400);
  
  win->MakeCurrent();
  
  //    stimuli
  std::string id = "model1";
  auto material1 = resource_manager.Create<Material>(id);
  auto mesh1 = resource_manager.Create<Mesh>(id);
  auto model1 = resource_manager.Create<Model>(id, mesh1, material1);

  model1->GetTransform().SetUnits(units::normalized);
  model1->GetTransform().SetScale(glm::vec3(0.5));
  model1->GetTransform().SetPosition(glm::vec3(0.5, 0.5, 1.0));

  material1->SetFaceColor(glm::vec3(1.0));

  mesh_factory::make_quad(mesh1.get());

  //  state initialization
  StateManager state_manager;
  StateRunner state_runner;
  
  auto state1 = state_manager.CreateState("first");
  state1->SetDuration(std::chrono::milliseconds(5000));
  
  state_runner.Begin(state1);
  
  while (!state_runner.Update() && !win->ShouldClose()) {
    renderer.Queue({*model1.get()}, win);
    renderer.Draw();
    renderer.ClearQueue();
    
    context_manager->PollEvents();
  }
}

void test1_run_all() {
  test1();
}
