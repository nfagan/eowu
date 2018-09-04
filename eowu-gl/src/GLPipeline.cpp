//
//  GLPipeline.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "GLPipeline.hpp"
#include "ContextManager.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "TextureManager.hpp"
#include <iostream>

eowu::GLPipeline::GLPipeline() {
  context_manager = std::make_shared<eowu::ContextManager>();
  renderer = std::make_shared<eowu::Renderer>(context_manager);
  resource_manager = std::make_shared<eowu::ResourceManager>();
  texture_manager = std::make_shared<eowu::TextureManager>();
}

std::shared_ptr<eowu::ContextManager> eowu::GLPipeline::GetContextManager() const {
  return context_manager;
}

std::shared_ptr<eowu::Renderer> eowu::GLPipeline::GetRenderer() const {
  return renderer;
}

std::shared_ptr<eowu::ResourceManager> eowu::GLPipeline::GetResourceManager() const {
  return resource_manager;
}

std::shared_ptr<eowu::TextureManager> eowu::GLPipeline::GetTextureManager() const {
  return texture_manager;
}

std::shared_ptr<eowu::GLPipeline> eowu::GLPipeline::GetInstance() {
  static std::mutex mut;
  static std::shared_ptr<eowu::GLPipeline> pipeline = nullptr;
  
  std::unique_lock<std::mutex> lock(mut);
  
  if (pipeline == nullptr) {
    pipeline = std::make_shared<eowu::GLPipeline>();
  }
  
  return pipeline;
}
