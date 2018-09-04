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

struct GLFWwindow;
struct GLFWmonitor;

namespace eowu {
  class Window;
  class ContextManager;
  
  namespace glfw {
    void window_size_callback(GLFWwindow *window, int width, int height);
  }
}

class eowu::Window {
  friend class ContextManager;
  
public:
  Window(GLFWmonitor *monitor, GLFWwindow *window, unsigned int width, unsigned int height);
  ~Window() = default;
  
  glm::vec2 GetFramebufferSize() const;
  glm::vec2 GetDimensions() const;
  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  const eowu::Identifier& GetIdentifier() const;
  
  void SetHeight(unsigned int height);
  void SetWidth(unsigned int width);
  
  bool IsOpen() const;
  bool WasResized() const;
  void MarkWasResized();
  void ResetWasResized();
  
  bool ShouldClose() const;
  void SwapBuffers() const;
  void MakeCurrent() const;
  void Show() const;
  
private:
  eowu::Identifier id;
  
  GLFWwindow *window;
  GLFWmonitor *monitor;
  
  unsigned int width;
  unsigned int height;
  
  bool is_open;
  bool was_resized;
  
  void configure_callbacks();
};
