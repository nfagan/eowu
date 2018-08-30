//
//  ContextManager.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "ContextManager.hpp"
#include "Error.hpp"

eowu::ContextManager::ContextManager() {
  is_initialized = false;
}

void eowu::ContextManager::Initialize() {
  if (IsInitialized()) {
    return;
  }
  
  if (!glfwInit()) {
    throw ContextFailedToInitializeError("GLFW initialization failed.");
  }
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  is_initialized = true;
}


bool eowu::ContextManager::IsInitialized() const {
  return is_initialized;
}

void eowu::ContextManager::CloseWindow(eowu::WindowType win) {
  glfwWindowShouldClose(win->window);
}

const eowu::WindowContainerType& eowu::ContextManager::GetWindows() const {
  return windows;
}

eowu::WindowType eowu::ContextManager::OpenWindow() {
  if (!IsInitialized()) {
    throw ContextNotInitializedError("OpenGL context has not been initialized.");
  }
  
  auto *primary = get_primary_monitor_with_trap();
  const auto *mode = glfwGetVideoMode(primary);
  
  auto width = mode->width;
  auto height = mode->height;
  
  GLFWwindow *other = nullptr;
  
  if (windows.size() > 0) {
    other = windows[0]->window;
  }
  
  GLFWwindow *window = glfwCreateWindow(width, height, "", nullptr, other);
  
  auto win = std::make_shared<eowu::Window>(primary, window, mode->width, mode->height);
  windows.push_back(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(eowu::u64 width, eowu::u64 height) {
  if (!IsInitialized()) {
    throw ContextNotInitializedError("OpenGL context has not been initialized.");
  }
  
  GLFWwindow *other = nullptr;
  
  if (windows.size() > 0) {
    other = windows[0]->window;
  }
  
  auto *primary = get_primary_monitor_with_trap();
  auto *window = glfwCreateWindow(width, height, "", nullptr, other);
  
  auto win = std::make_shared<eowu::Window>(primary, window, width, height);
  
  windows.push_back(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(eowu::u32 index) {
  if (!IsInitialized()) {
    throw ContextNotInitializedError("OpenGL context has not been initialized.");
  }
  
  auto *monitor = get_monitor_with_trap(index);
  const auto *mode = glfwGetVideoMode(monitor);
  
  auto width = mode->width;
  auto height = mode->height;
  
  GLFWwindow *other = nullptr;
  
  if (windows.size() > 0) {
    other = windows[0]->window;
  }
  
  GLFWwindow *window = glfwCreateWindow(width, height, "", monitor, other);
  
  auto win = std::make_shared<eowu::Window>(monitor, window, width, height);
  
  windows.push_back(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(eowu::u32 index, eowu::u64 width, eowu::u64 height) {
  if (!IsInitialized()) {
    throw ContextNotInitializedError("OpenGL context has not been initialized.");
  }
  
  GLFWwindow *other = nullptr;
  
  if (windows.size() > 0) {
    other = windows[0]->window;
  }
  
  auto *monitor = get_monitor_with_trap(index);
  auto *window = glfwCreateWindow(width, height, "", monitor, other);
  
  auto win = std::make_shared<eowu::Window>(monitor, window, width, height);
  
  windows.push_back(win);
  
  return win;
}

GLFWmonitor* eowu::ContextManager::get_primary_monitor_with_trap() const {
  GLFWmonitor *primary = glfwGetPrimaryMonitor();
  
  if (!primary) {
    throw MonitorNotAvailableError("No monitors are available.");
  }
  
  return primary;
}

GLFWmonitor* eowu::ContextManager::get_monitor_with_trap(eowu::u32 index) const {
  int count;
  
  auto **monitors = glfwGetMonitors(&count);
  
  if (index >= count) {
    throw MonitorNotAvailableError("The requested monitor index is out of bounds.");
  }
  
  return monitors[index];
}
