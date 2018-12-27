//
//  GLPipeline.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>
#include <mutex>

namespace eowu {
  class Renderer;
  class ContextManager;
  class ResourceManager;
  class TextureManager;
  class WindowContainerMap;
  class FontManager;
  
  class GLPipeline;
}

class eowu::GLPipeline {
public:
  static std::shared_ptr<GLPipeline> GetInstance();
  
  GLPipeline();
  
  std::shared_ptr<eowu::ContextManager> GetContextManager() const;
  std::shared_ptr<eowu::Renderer> GetRenderer() const;
  std::shared_ptr<eowu::ResourceManager> GetResourceManager() const;
  std::shared_ptr<eowu::TextureManager> GetTextureManager() const;
  std::shared_ptr<eowu::WindowContainerMap> GetWindowContainer() const;
  std::shared_ptr<eowu::FontManager> GetFontManager() const;
  
private:
  std::shared_ptr<eowu::ContextManager> context_manager;
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::ResourceManager> resource_manager;
  std::shared_ptr<eowu::TextureManager> texture_manager;
  std::shared_ptr<eowu::FontManager> font_manager;
  std::shared_ptr<eowu::WindowContainerMap> window_container;
};
