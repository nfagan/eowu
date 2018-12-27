//
//  Window.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Identifier.hpp"
#include "XYSource.hpp"
#include "Keyboard.hpp"
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
    unsigned int swap_interval;
    
    bool is_fullscreen;
    bool is_resizeable;
    
    std::string title;
    std::string alias;
    
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
  
  glm::ivec2 GetFramebufferSize() const;
  glm::vec2 GetDimensions() const;
  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  const eowu::Identifier& GetIdentifier() const;
  const eowu::XYSource& GetMouse() const;
  eowu::Keyboard& GetKeyboard();
  const std::string& GetAlias() const;
  
  void Close();
  
  void SetAlias(const std::string &alias);
  void SetHeight(unsigned int height);
  void SetWidth(unsigned int width);
  void SetPosition(unsigned int x, unsigned int y);
  void SetSwapInterval(unsigned int interval);
  
  void ApplySwapInterval() const;
  
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
  std::string alias;
  
  GLFWmonitor *monitor;
  GLFWwindow *window;
  
  eowu::XYSource mouse;
  eowu::Keyboard keyboard;
  
  std::atomic<unsigned int> width;
  std::atomic<unsigned int> height;
  std::atomic<unsigned int> swap_interval;
  
  std::atomic<bool> is_open;
  std::atomic<bool> is_valid;
  std::atomic<bool> was_resized;
  
  void mark_closed();
};
