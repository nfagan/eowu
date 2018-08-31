//
//  test-util.hpp
//  eowu
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

namespace {
  std::vector<float> get_values() {
    std::vector<float> values;
    values.push_back(0.1);
    values.push_back(0.2);
    
    return values;
  }
  
  eowu::Attribute get_position_attribute() {
    auto values = get_values();
    return eowu::Attribute(eowu::attributes::position, values);
  }
  
  eowu::Attribute get_texture_attribute() {
    auto values = get_values();
    return eowu::Attribute(eowu::attributes::uv, values);
  }
  
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

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window) {
      glfwTerminate();
      return nullptr;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    
    return window;
  }
}
