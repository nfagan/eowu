//
//  FontFace.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/28/18.
//

#pragma once

#include "FontPlatform.hpp"
#include <eowu-common/types.hpp>
#include <string>

namespace eowu {
  class FontFace;
  class FontManager;
}

class eowu::FontFace {
  friend class eowu::FontManager;
public:
  struct PixelSizes {
    eowu::u32 width;
    eowu::u32 height;
    
    PixelSizes();
    ~PixelSizes() = default;
    PixelSizes(eowu::u32 w, eowu::u32 h);
    
    static eowu::FontFace::PixelSizes AutoWidth(eowu::u32 height);
  };
public:
  FontFace(eowu::FontManager *font_manager);
  ~FontFace();
  
  void SetPixelSizes(eowu::FontFace::PixelSizes pixel_sizes);
  
private:
  eowu::FontManager *font_manager;
  FT_Face font_face;
  bool is_initialized;
  
private:
  void open(const std::string &file, eowu::s32 face_index = 0);
  
  void assert_initialized();
};
