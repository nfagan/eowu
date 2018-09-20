//
//  Window.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Window.hpp"
#include "Error.hpp"
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <assert.h>

eowu::Window::Window(GLFWmonitor *monitor_, GLFWwindow *window_, unsigned int width_, unsigned int height_) :
monitor(monitor_), window(window_), width(width_), height(height_),
swap_interval(1), is_open(true), is_valid(true), was_resized(false) {
  //
}

eowu::Window::~Window() {
  Close();
}

const eowu::Identifier& eowu::Window::GetIdentifier() const {
  return id;
}

eowu::Keyboard& eowu::Window::GetKeyboard() {
  return keyboard;
}

const eowu::XYSource& eowu::Window::GetMouse() const {
  return mouse;
}

void eowu::Window::SetWidth(unsigned int width) {
  this->width = width;
}

void eowu::Window::SetHeight(unsigned int height) {
  this->height = height;
}

void eowu::Window::SetSwapInterval(unsigned int interval) {
  this->swap_interval = interval;
}

void eowu::Window::ApplySwapInterval() const {
  glfwSwapInterval(swap_interval);
}

glm::ivec2 eowu::Window::GetFramebufferSize() const {
  assert(window);
  
  int width;
  int height;
  
  glfwGetFramebufferSize(window, &width, &height);
  
  return glm::ivec2(width, height);
}

glm::vec2 eowu::Window::GetDimensions() const {
  return glm::vec2(width.load(), height.load());
}

unsigned int eowu::Window::GetWidth() const {
  return width;
}

unsigned int eowu::Window::GetHeight() const {
  return height;
}

bool eowu::Window::IsOpen() const {
  return is_open && is_valid;
}

bool eowu::Window::WasResized() const {
  return was_resized;
}

void eowu::Window::MarkWasResized() {
  was_resized = true;
}

void eowu::Window::ResetWasResized() {
  was_resized = false;
}

void eowu::Window::Close() {
  if (!is_open || !is_valid) {
    return;
  }
  
  mark_closed();
  
  glfwSetWindowShouldClose(window, true);
}

void eowu::Window::mark_closed() {
  is_open = false;
  is_valid = false;
  
  mouse.SetIsValid(false);
}

bool eowu::Window::ShouldClose() const {
  return !is_open || !is_valid || glfwWindowShouldClose(window);
}

void eowu::Window::SwapBuffers() const {
  glfwSwapBuffers(window);
}

void eowu::Window::MakeCurrent() const {
  glfwMakeContextCurrent(window);
}

void eowu::Window::Show() const {
  glfwShowWindow(window);
}

void eowu::Window::SetPosition(unsigned int x, unsigned int y) {
  if (!IsOpen()) {
    throw eowu::WindowError("Attempted to position a closed window.");
  }
  
  glfwSetWindowPos(window, x, y);
}

void eowu::Window::SetAlias(const std::string &alias) {
  this->alias = alias;
}

const std::string& eowu::Window::GetAlias() const {
  return alias;
}


//
//  window properties
//

eowu::WindowProperties::WindowProperties() :
width(0), height(0), index(0), swap_interval(1), is_fullscreen(true), is_resizeable(true) {
  //
}

eowu::WindowProperties::WindowProperties(unsigned int width_, unsigned int height_) :
width(width_), height(height_), index(0), swap_interval(1), is_fullscreen(false), is_resizeable(true) {
  //
}

eowu::WindowProperties::WindowProperties(unsigned int index_, unsigned int width_, unsigned int height_) :
width(width_), height(height_), index(index_), swap_interval(1), is_fullscreen(false), is_resizeable(true) {
  //
}

eowu::WindowProperties::WindowProperties(unsigned int index_) :
width(0), height(0), index(index_), swap_interval(1), is_fullscreen(true), is_resizeable(false) {
  //
}


