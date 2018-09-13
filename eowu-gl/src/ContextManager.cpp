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
#include <chrono>

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

void eowu::ContextManager::CloseWindows() {
  for (auto &win : windows) {
    win->Close();
  }
}

bool eowu::ContextManager::AllShouldClose() const {
  for (const auto &win : windows) {
    if (!win->ShouldClose()) {
      return false;
    }
  }
  
  return true;
}

bool eowu::ContextManager::AnyShouldClose() const {
  for (const auto &win : windows) {
    if (win->ShouldClose()) {
      return true;
    }
  }
  
  return false;
}

void eowu::ContextManager::PollEvents() const {
  glfwPollEvents();
}

bool eowu::ContextManager::IsInitialized() const {
  return is_initialized;
}

const eowu::WindowContainerType& eowu::ContextManager::GetWindows() const {
  return windows;
}

eowu::WindowType eowu::ContextManager::OpenWindow(const eowu::WindowProperties &props) {
  if (!IsInitialized()) {
    throw ContextNotInitializedError("OpenGL context has not been initialized.");
  }
  
  auto *monitor = get_monitor_with_trap(props.index);
  const auto *mode = glfwGetVideoMode(monitor);
  
  GLFWwindow *other = nullptr;
  GLFWwindow *new_window = nullptr;
  
  if (windows.size() > 0) {
    other = windows[0]->window;
  }
  
  glfwWindowHint(GLFW_RESIZABLE, props.is_resizeable);
  
  const char* const title = props.title.c_str();
  
  if (props.is_fullscreen) {
    new_window = glfwCreateWindow(mode->width, mode->height, title, monitor, other);
  } else {
    new_window = glfwCreateWindow(props.width, props.height, title, nullptr, other);
  }
  
  assert(new_window);
  
  int client_width;
  int client_height;
  
  glfwGetWindowSize(new_window, &client_width, &client_height);
  
  auto win = std::make_shared<eowu::Window>(monitor, new_window, client_width, client_height);
  
  win->SetSwapInterval(props.swap_interval);
  
  register_window(win);
  
  return win;
  
}

GLFWmonitor* eowu::ContextManager::get_monitor_with_trap(unsigned int index) const {
  int count;
  
  auto **monitors = glfwGetMonitors(&count);
  
  if (index >= count) {
    throw MonitorNotAvailableError("The requested monitor index (" + std::to_string(index) + ") is out of bounds.");
  }
  
  return monitors[index];
}

std::size_t eowu::ContextManager::find_window_with_trap(GLFWwindow *win) {
  std::size_t n_windows = windows.size();
  
  for (std::size_t i = 0; i < n_windows; i++) {
    const auto *owned_window = windows[i].get();
    
    if (owned_window->window == win) {
      return i;
    }
  }
  
  throw eowu::InteralContextError("Window was not found.");
}

eowu::WindowType eowu::ContextManager::get_window_with_trap(GLFWwindow *win) {
  std::size_t index = find_window_with_trap(win);
  return windows[index];
}

void eowu::ContextManager::register_window(eowu::WindowType win) {
  windows.push_back(win);
  
  if (!loaded_gl_pointers) {
    win->MakeCurrent();
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    loaded_gl_pointers = true;
  }
  
  configure_window_callbacks(win->window);
  
  //  Seems to be necessary to enable window display
  //  in macOS :(
  PollEvents();
}

void eowu::ContextManager::configure_window_callbacks(GLFWwindow *win) {
  glfwSetWindowUserPointer(win, (void*)this);
  
  glfwSetWindowSizeCallback(win, eowu::glfw::window_size_callback);
  glfwSetWindowCloseCallback(win, eowu::glfw::window_close_callback);
  glfwSetCursorPosCallback(win, eowu::glfw::window_mouse_position_callback);
  glfwSetKeyCallback(win, eowu::glfw::window_key_press_callback);
}

//  mouse position
void eowu::glfw::window_mouse_position_callback(GLFWwindow *window, double x, double y) {
  eowu::ContextManager* context = (eowu::ContextManager*)glfwGetWindowUserPointer(window);
  
  assert(context);
  
  auto win = context->get_window_with_trap(window);
}

//  key press
void eowu::glfw::window_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  eowu::ContextManager* context = (eowu::ContextManager*)glfwGetWindowUserPointer(window);
  
  assert(context);
  
  auto win = context->get_window_with_trap(window);
  
  std::cout << std::boolalpha << (action == GLFW_REPEAT) << std::endl;
}

//  window close
void eowu::glfw::window_close_callback(GLFWwindow *window) {
  eowu::ContextManager* context = (eowu::ContextManager*)glfwGetWindowUserPointer(window);
  
  assert(context);
  
  auto win = context->get_window_with_trap(window);
  win->mark_closed();
  
  glfwDestroyWindow(win->window);
}

//  window resize
void eowu::glfw::window_size_callback(GLFWwindow *window, int width, int height) {
  eowu::ContextManager* context = (eowu::ContextManager*)glfwGetWindowUserPointer(window);
  
  assert(context);
  
  auto win = context->get_window_with_trap(window);
  
  win->SetWidth(width);
  win->SetHeight(height);
  win->MarkWasResized();
}
