//
//  WindowContainerMap.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#include "WindowContainerMap.hpp"
#include "Window.hpp"
#include "Error.hpp"

void eowu::WindowContainerMap::Emplace(const std::string &id, std::shared_ptr<eowu::Window> window) {
  std::unique_lock<std::mutex> lock(mut);
  windows.emplace(id, window);
  keys.push_back(id);
}

bool eowu::WindowContainerMap::Has(const std::string &id) const {
  std::unique_lock<std::mutex> lock(mut);
  return windows.count(id) > 0;
}

std::size_t eowu::WindowContainerMap::Size() const {
  std::lock_guard<std::mutex> lock(mut);
  return windows.size();
}

std::vector<std::string> eowu::WindowContainerMap::Keys() const {
  return keys;
}

std::shared_ptr<eowu::Window> eowu::WindowContainerMap::Get() const {
  std::unique_lock<std::mutex> lock(mut);
  
  if (windows.size() == 0) {
    throw eowu::NonexistentResourceError("No windows are present.");
  }
  
  return windows.begin()->second;
}

std::shared_ptr<eowu::Window> eowu::WindowContainerMap::Get(const std::string &id) const {
  std::unique_lock<std::mutex> lock(mut);
  
  const auto &win_it = windows.find(id);
  
  if (win_it == windows.end()) {
    throw eowu::NonexistentResourceError("Window: '" + id + "' does not exist.");
  }
  
  return win_it->second;
}
