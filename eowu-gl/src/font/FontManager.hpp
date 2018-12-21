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
  
  std::shared_ptr<eowu::FontFace> GetFontFace(const std::string &filename, eowu::s32 face_index = 0);
  
private:
  FT_Library ft_library;
  bool is_initialized;
  
private:
  void assert_initialized();
};
