//
//  ContextManager.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ContextManager.hpp"
#include "Error.hpp"
#include <iostream>

eowu::ContextManager::ContextManager() {
  is_initialized = false;
  loaded_gl_pointers = false;
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

void eowu::ContextManager::PollEvents() const {
  glfwPollEvents();
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
  
  register_window(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(unsigned int width, unsigned int height) {
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
  
  register_window(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(unsigned int index) {
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
  
  register_window(win);
  
  return win;
}

eowu::WindowType eowu::ContextManager::OpenWindow(unsigned int index, unsigned int width, unsigned int height) {
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
  
  register_window(win);
  
  return win;
}

GLFWmonitor* eowu::ContextManager::get_primary_monitor_with_trap() const {
  GLFWmonitor *primary = glfwGetPrimaryMonitor();
  
  if (!primary) {
    throw MonitorNotAvailableError("No monitors are available.");
  }
  
  return primary;
}

GLFWmonitor* eowu::ContextManager::get_monitor_with_trap(unsigned int index) const {
  int count;
  
  auto **monitors = glfwGetMonitors(&count);
  
  if (index >= count) {
    throw MonitorNotAvailableError("The requested monitor index is out of bounds.");
  }
  
  return monitors[index];
}

void eowu::ContextManager::register_window(eowu::WindowType win) {
  windows.push_back(win);
  
  if (!loaded_gl_pointers) {
    win->MakeCurrent();
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    loaded_gl_pointers = true;
  }
  
  //  Seems to be necessary to enable window display
  //  in macOS :(
  PollEvents();
}
