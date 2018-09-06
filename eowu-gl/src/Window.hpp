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
#include <string>
#include <atomic>

struct GLFWwindow;
struct GLFWmonitor;

namespace eowu {
  class Window;
  class ContextManager;
  
  namespace glfw {
    void window_close_callback(GLFWwindow *window);
  }
  
  struct WindowProperties {
    unsigned int width;
    unsigned int height;
    unsigned int index;
    bool is_fullscreen;
    std::string title;
    
    WindowProperties();
    WindowProperties(unsigned int width, unsigned int height);
    WindowProperties(unsigned int index, unsigned int width, unsigned int height);
    WindowProperties(unsigned int index);
  };
}

class eowu::Window {
  friend class ContextManager;
  
public:
  Window(GLFWmonitor *monitor, GLFWwindow *window, unsigned int width, unsigned int height);
  ~Window();
  
  glm::vec2 GetFramebufferSize() const;
  glm::vec2 GetDimensions() const;
  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  const eowu::Identifier& GetIdentifier() const;
  
  void Close();
  
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
  
  friend void eowu::glfw::window_close_callback(GLFWwindow *window);
  
private:
  eowu::Identifier id;
  
  GLFWwindow *window;
  GLFWmonitor *monitor;
  
  std::atomic<unsigned int> width;
  std::atomic<unsigned int> height;
  
  std::atomic<bool> is_open;
  std::atomic<bool> was_resized;
  std::atomic<bool> is_valid;
  
  void mark_closed();
};
