//
//  Keyboard.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <unordered_map>
#include <mutex>
#include <string>

struct GLFWwindow;

namespace eowu {
  class Keyboard;
  class ContextManager;
  
  namespace glfw {
    void window_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
  }
}

class eowu::Keyboard {
  friend class eowu::ContextManager;
  
public:
  Keyboard() = default;
  ~Keyboard() = default;
  
  bool IsPressed(int key);
  bool IsPressed(const std::string &key);
  
  bool WasPressed(int key);
  bool WasPressed(const std::string &key);
  
  std::unordered_map<int, bool> GetState() const;
  
  static int GetKeyCode(const std::string &name);
  static bool IsKeyName(const std::string &name);
  
  friend void eowu::glfw::window_key_press_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
private:
  mutable std::mutex mut;
  
  std::unordered_map<int, eowu::u64> pressed_counts;
  std::unordered_map<int, bool> pressed_state;
  
  static const std::unordered_map<std::string, int> key_map;
  
  void mark_pressed(int key);
  void mark_released(int key);
};
