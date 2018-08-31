//
//  ContextManager.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

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
  
  eowu::WindowType OpenWindow();
  eowu::WindowType OpenWindow(eowu::u32 index);
  eowu::WindowType OpenWindow(eowu::u32 index, eowu::u64 width, eowu::u64 height);
  eowu::WindowType OpenWindow(eowu::u64 width, eowu::u64 height);
  
  void CloseWindow(eowu::WindowType win);
  
  const WindowContainerType& GetWindows() const;
  
  bool IsInitialized() const;
  
private:
  bool is_initialized;
  bool loaded_gl_pointers;
  
  GLFWmonitor* get_monitor_with_trap(eowu::u32 index) const;
  GLFWmonitor* get_primary_monitor_with_trap() const;
  
  WindowContainerType windows;
  
  void register_window(eowu::WindowType win);
};
