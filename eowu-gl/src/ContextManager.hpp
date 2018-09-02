//
//  ContextManager.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Window.hpp"
#include <memory>
#include <vector>

struct GLFWmonitor;

namespace eowu {
  class ContextManager;
  
  using WindowType = std::shared_ptr<Window>;
  using WindowContainerType = std::vector<WindowType>;
}

class eowu::ContextManager {
public:
  ContextManager();
  ~ContextManager() = default;
  
  void Initialize();
  bool IsInitialized() const;
  
  void PollEvents() const;
  
  eowu::WindowType OpenWindow();
  eowu::WindowType OpenWindow(unsigned int index);
  eowu::WindowType OpenWindow(unsigned int index, unsigned int width, unsigned int height);
  eowu::WindowType OpenWindow(unsigned int width, unsigned int height);
  
  const WindowContainerType& GetWindows() const;
  void CloseWindow(eowu::WindowType win);
private:
  bool is_initialized;
  bool loaded_gl_pointers;
  
  GLFWmonitor* get_monitor_with_trap(unsigned int index) const;
  GLFWmonitor* get_primary_monitor_with_trap() const;
  
  WindowContainerType windows;
  
  void register_window(eowu::WindowType win);
};
