//
//  Window.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Window.hpp"

eowu::Window::Window(GLFWmonitor *monitor, GLFWwindow *window, eowu::u64 width, eowu::u64 height) {
  this->monitor = monitor;
  this->window = window;
  this->width = width;
  this->height = height;
}

glm::vec2 eowu::Window::GetDimensions() const {
  return glm::vec2(width, height);
}

eowu::u64 eowu::Window::GetWidth() const {
  return width;
}

eowu::u64 eowu::Window::GetHeight() const {
  return height;
}
