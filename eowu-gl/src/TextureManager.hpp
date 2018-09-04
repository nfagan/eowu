//
//  TextureManager.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#pragma once

#include <unordered_map>
#include <mutex>
#include <string>

namespace eowu {
  class Texture;
  class TextureManager;
}

class eowu::TextureManager {
public:
  TextureManager() = default;
  ~TextureManager();
  
  void LoadImage(const std::string &filename, const std::string &id);
  eowu::Texture Get(const std::string &id) const;
  
private:
  mutable std::mutex mut;
  
  std::unordered_map<std::string, eowu::Texture> textures;
};
