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
#include <unordered_map>

namespace eowu {
  class FontGlyph;
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
  
  const eowu::FontGlyph& GetGlyph(const std::string &character) const;
  const eowu::FontGlyph& GetGlyph(char character) const;
  
  bool HasGlyph(const std::string &character) const;
  bool HasGlyph(char character) const;
  
  void LoadGlyphsAscii();
private:
  eowu::FontManager *font_manager;
  FT_Face font_face;
  bool is_initialized;
  bool is_deleted;
  
  std::unordered_map<std::string, eowu::FontGlyph> glyphs;
private:
  void open(const std::string &file, eowu::s32 face_index = 0);
  void dispose();
  
  void LoadGlyphs(const std::string &characters);
  
  void assert_initialized();
};
