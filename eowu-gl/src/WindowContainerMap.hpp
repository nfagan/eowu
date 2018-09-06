//
//  WindowContainerMap.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <string>

namespace eowu {
  class Window;
  class WindowContainerMap;
}

class eowu::WindowContainerMap {
public:
  WindowContainerMap() = default;
  ~WindowContainerMap() = default;
  
  void Emplace(const std::string &id, std::shared_ptr<eowu::Window> window);
  std::shared_ptr<eowu::Window> Get(const std::string &id) const;
  bool Has(const std::string &id) const;
  
private:
  mutable std::mutex mut;
  std::unordered_map<std::string, std::shared_ptr<eowu::Window>> windows;
};


