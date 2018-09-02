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

GLFWwindow* init_glfw() {
  GLFWwindow* window;
  
  if (!glfwInit()) {
    return nullptr;
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
  
  if (!window) {
    glfwTerminate();
    return nullptr;
  }
  
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
//  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  
  return window;
}

void test2() {
  auto* window = init_glfw();
  
  if (!window) {
    std::cout << "ERROR: Failed to initialize glfw" << std::endl;
    return;
  }
  
  while (!glfwWindowShouldClose(window))
  {
//    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
}

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
  model1->GetTransform().SetPosition(glm::vec3(0.0, 1.0, 1.0));

  material1->SetFaceColor(glm::vec3(1.0));

  mesh_factory::make_quad(mesh1.get());

  //  state initialization
  
  StateManager state_manager;
  StateRunner state_runner;
  
  auto state1 = state_manager.CreateState("first");
  state1->SetDuration(std::chrono::milliseconds(5000));
  
  state_runner.Next(state1);
  
  while (!state_runner.Update()) {
    renderer.Queue({*model1.get()});
    renderer.Draw(win);
    renderer.ClearQueue();
  }
}

void test1_run_all() {
  test1();
}
