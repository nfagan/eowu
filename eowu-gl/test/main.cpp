#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <eowu-gl/eowu-gl.hpp>
#include <iostream>
#include <vector>

#include "test-vertex.hpp"
#include "test-mesh.hpp"

void test_glfw() {
  GLFWwindow* window;
  
  /* Initialize the library */
  if (!glfwInit())
    return;
  
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return;
  }
  
  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    
    /* Poll for and process events */
    glfwPollEvents();
  }
  
  glfwTerminate();
}

int main(int argc, char** argv) {
  
  test_vertex_run_all();
  test_mesh_run_all();
  test_glfw();
  
  return 0;
}
