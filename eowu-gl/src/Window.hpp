//
//  Window.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Identifier.hpp"
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
  Window(GLFWmonitor *monitor, GLFWwindow *window, unsigned int width, unsigned int height);
  
  glm::vec2 GetDimensions() const;
  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  const eowu::Identifier& GetIdentifier() const;
  
  bool ShouldClose() const;
  void SwapBuffers() const;
  void MakeCurrent() const;
  
private:
  eowu::Identifier id;
  
  GLFWwindow *window;
  GLFWmonitor *monitor;
  
  unsigned int width;
  unsigned int height;
};
