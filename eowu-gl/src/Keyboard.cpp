//
//  Keyboard.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#include "Keyboard.hpp"
#include <GLFW/glfw3.h>

void eowu::Keyboard::mark_pressed(int key) {
  std::unique_lock<std::mutex> lock(mut);
  
  auto it = pressed_counts.find(key);
  
  eowu::u64 n_pressed;
  
  //  Not yet marked.
  if (it == pressed_counts.end()) {
    n_pressed = 1;
  } else {
    n_pressed = it->second + 1;
  }
  
  pressed_counts[key] = n_pressed;
  pressed_state[key] = true;
}

void eowu::Keyboard::mark_released(int key) {
  std::unique_lock<std::mutex> lock(mut);
  
  pressed_state[key] = false;
}

bool eowu::Keyboard::IsPressed(int key) {
  std::unique_lock<std::mutex> lock(mut);
  
  if (pressed_state.count(key) == 0) {
    return false;
  }
  
  return pressed_state.at(key);
}

bool eowu::Keyboard::IsPressed(const std::string &key) {
  return IsPressed(GetKeyCode(key));
}

bool eowu::Keyboard::WasPressed(int key) {
  std::unique_lock<std::mutex> lock(mut);
  
  if (pressed_counts.count(key) == 0) {
    return false;
  }
  
  eowu::u64 n_pressed = pressed_counts.at(key);
  
  if (n_pressed == 0) {
    return false;
  }
  
//  pressed_counts[key] = n_pressed - 1;
  pressed_counts[key] = 0;
  
  return true;
}

bool eowu::Keyboard::WasPressed(const std::string &key) {
  return WasPressed(GetKeyCode(key));
}

std::unordered_map<int, bool> eowu::Keyboard::GetState() const {
  std::unique_lock<std::mutex> lock(mut);
  
  auto copy = pressed_state;
  
  return copy;
}

int eowu::Keyboard::GetKeyCode(const std::string &name) {
  return key_map.at(name);
}

bool eowu::Keyboard::IsKeyName(const std::string &name) {
  return key_map.count(name) > 0;
}

const std::unordered_map<std::string, int> eowu::Keyboard::key_map{
  {"0", GLFW_KEY_0},
  {"1", GLFW_KEY_1},
  {"2", GLFW_KEY_2},
  {"3", GLFW_KEY_3},
  {"4", GLFW_KEY_4},
  {"5", GLFW_KEY_5},
  {"6", GLFW_KEY_6},
  {"7", GLFW_KEY_7},
  {"8", GLFW_KEY_8},
  {"9", GLFW_KEY_9},
  //
  {"a", GLFW_KEY_A},
  {"b", GLFW_KEY_B},
  {"c", GLFW_KEY_C},
  {"d", GLFW_KEY_D},
  {"e", GLFW_KEY_E},
  {"f", GLFW_KEY_F},
  {"g", GLFW_KEY_G},
  {"h", GLFW_KEY_H},
  {"i", GLFW_KEY_I},
  {"j", GLFW_KEY_J},
  {"k", GLFW_KEY_K},
  {"l", GLFW_KEY_L},
  {"m", GLFW_KEY_M},
  {"n", GLFW_KEY_N},
  {"o", GLFW_KEY_O},
  {"p", GLFW_KEY_P},
  {"q", GLFW_KEY_Q},
  {"r", GLFW_KEY_R},
  {"s", GLFW_KEY_S},
  {"t", GLFW_KEY_T},
  {"u", GLFW_KEY_U},
  {"v", GLFW_KEY_V},
  {"w", GLFW_KEY_W},
  {"x", GLFW_KEY_X},
  {"y", GLFW_KEY_Y},
  {"z", GLFW_KEY_Z},
  //
  {"up", GLFW_KEY_UP},
  {"down", GLFW_KEY_DOWN},
  {"left", GLFW_KEY_LEFT},
  {"right", GLFW_KEY_RIGHT},
  //
  {"space", GLFW_KEY_SPACE},
  {"escape", GLFW_KEY_ESCAPE},
  {"enter", GLFW_KEY_ENTER},
  {"left-shift", GLFW_KEY_LEFT_SHIFT},
  {"right-shift", GLFW_KEY_RIGHT_SHIFT}
};
