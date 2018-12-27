//
//  FontManager.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/28/18.
//

#pragma once

#include "FontPlatform.hpp"
#include <eowu-common/types.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace eowu {
  class FontManager;
  class FontFace;
}

class eowu::FontManager {
  friend class FontFace;
  
public:
  FontManager();
  ~FontManager();
  
  void Initialize();
  
  std::shared_ptr<eowu::FontFace> LoadFontFace(const std::string &filename,
                                               const std::string &alias,
                                               eowu::s32 face_index = 0);
  bool HasFontFace(const std::string &alias) const;
  std::shared_ptr<eowu::FontFace> GetFontFace(const std::string &alias);
  
private:
  FT_Library ft_library;
  std::unordered_map<std::string, std::shared_ptr<eowu::FontFace>> font_faces;
  bool is_initialized;
private:
  void assert_initialized();
};
