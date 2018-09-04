//
//  TextureManager.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/4/18.
//

#include "TextureManager.hpp"
#include "Texture.hpp"
#include "TextureLoader.hpp"
#include "Error.hpp"

eowu::TextureManager::~TextureManager() {
  for (const auto &it : textures) {
    it.second.Dispose();
  }
}

void eowu::TextureManager::LoadImage(const std::string &filename, const std::string &id) {
  std::unique_lock<std::mutex> lock(mut);
  
  auto tex = eowu::load::image(filename);
  
  textures[id] = tex;
}

eowu::Texture eowu::TextureManager::Get(const std::string &id) const {
  std::unique_lock<std::mutex> lock(mut);
  
  return textures.at(id);
}
