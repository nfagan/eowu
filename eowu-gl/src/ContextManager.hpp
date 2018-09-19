//
//  ContextManager.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Window.hpp"
#include "Keyboard.hpp"
#include <memory>
#include <vector>
#include <cstddef>

struct GLFWmonitor;
struct GLFWwindow;

namespace eowu {
  class ContextManager;
  
  using WindowType = std::shared_ptr<Window>;
  using WindowContainerType = std::vector<WindowType>;
  
  namespace glfw {
    void window_size_callback(GLFWwindow *window, int width, int height);
    void window_close_callback(GLFWwindow *window);
    void window_mouse_position_callback(GLFWwindow *window, double x, double y);
    void window_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
  }
}

class eowu::ContextManager {
public:
  ContextManager();
  ~ContextManager() = default;
  
  void Initialize();
  bool IsInitialized() const;
  void PollEvents() const;
  
  template<typename ...T>
  eowu::WindowType OpenWindow(T... args);
  eowu::WindowType OpenWindow(const eowu::WindowProperties &props);
  
  void CloseWindows();
  bool AnyShouldClose() const;
  bool AllShouldClose() const;
  
  const WindowContainerType& GetWindows() const;
  const WindowType& GetWindowByAlias(const std::string &alias) const;
  eowu::Keyboard& GetKeyboard();
  
  friend void eowu::glfw::window_size_callback(GLFWwindow *window, int width, int height);
  friend void eowu::glfw::window_close_callback(GLFWwindow *window);
  friend void eowu::glfw::window_mouse_position_callback(GLFWwindow *window, double x, double y);
  friend void eowu::glfw::window_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
private:
  bool is_initialized;
  bool loaded_gl_pointers;
  
  GLFWmonitor* get_monitor_with_trap(unsigned int index) const;
  GLFWmonitor* get_primary_monitor_with_trap() const;
  
  WindowContainerType windows;
  eowu::Keyboard keyboard;
  
  std::size_t find_window_with_trap(GLFWwindow *win);
  eowu::WindowType get_window_with_trap(GLFWwindow *win);
  
  void register_window(eowu::WindowType win);
  void configure_window_callbacks(GLFWwindow *win);
  void update_mouse_position(const WindowType &win, const eowu::Coordinate &coord);
};

template<typename ...T>
eowu::WindowType eowu::ContextManager::OpenWindow(T... args) {
  eowu::WindowProperties props(args...);
  
  return OpenWindow(props);
}
