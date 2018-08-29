//
//  Window.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace eowu {
  class Window;
  class ContextManager;
}

class eowu::Window {
  friend class ContextManager;
  
public:
  Window(GLFWmonitor *monitor, GLFWwindow *window, eowu::u64 width, eowu::u64 height);
  
  glm::vec2 GetDimensions() const;
  eowu::u64 GetWidth() const;
  eowu::u64 GetHeight() const;
  
private:
  GLFWwindow *window;
  GLFWmonitor *monitor;
  
  eowu::u64 width;
  eowu::u64 height;
};
