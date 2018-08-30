#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <eowu-gl/eowu-gl.hpp>
#include <iostream>
#include <vector>

#include "test-util.hpp"
#include "test-vertex.hpp"
#include "test-mesh.hpp"
#include "test-shader.hpp"
#include "test-uniform.hpp"
#include "test-context.hpp"
#include "test-shader-builder.hpp"
#include "test-renderer.hpp"
#include "test-identifier.hpp"

void test_glfw() {
  auto* window = init_glfw();
  
  if (!window) {
    std::cout << "ERROR: Failed to initialize glfw" << std::endl;
    return;
  }
  
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  
  glfwTerminate();
}

int main(int argc, char** argv) {
  
  test_identifier_run_all();
  test_renderer_run_all();
  test_vertex_run_all();
  test_mesh_run_all();
  test_shader_run_all();
  test_uniform_run_all();
  test_context_run_all();
  test_shader_builder_run_all();
//  test_glfw();
  
  return 0;
}
