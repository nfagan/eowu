//
//  FontFace.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/28/18.
//

#include "FontFace.hpp"
#include "FontManager.hpp"
#include "../Error.hpp"

eowu::FontFace::FontFace(eowu::FontManager *manager) :
font_manager(manager), is_initialized(false) {
  //
}

eowu::FontFace::~FontFace() {
  FT_Done_Face(font_face);
}

void eowu::FontFace::SetPixelSizes(eowu::FontFace::PixelSizes pixel_sizes) {
  assert_initialized();
  
  auto res = FT_Set_Pixel_Sizes(font_face, pixel_sizes.width, pixel_sizes.height);
  
  if (res) {
    throw eowu::FontError("Failed to set pixel sizes for font face.");
  }
}

void eowu::FontFace::open(const std::string &file, eowu::s32 face_index) {
  if (is_initialized) {
    throw eowu::FontError("Font is already initialized.");
  }
  
  auto res = FT_New_Face(font_manager->ft_library, file.c_str(), face_index, &font_face);
  
  if (res) {
    throw eowu::FontError("Failed to initialize font face: " + file);
  }
  
  is_initialized = true;
}

void eowu::FontFace::assert_initialized() {
  if (!is_initialized) {
    throw eowu::FontError("Font face is not yet initialized.");
  }
}

//
//
//

eowu::FontFace::PixelSizes::PixelSizes() :
width(0), height(0) {
  //
}

eowu::FontFace::PixelSizes::PixelSizes(eowu::u32 w, eowu::u32 h) :
width(w), height(h) {
  //
}

eowu::FontFace::PixelSizes eowu::FontFace::PixelSizes::AutoWidth(eowu::u32 height) {
  eowu::FontFace::PixelSizes ps(0, height);
  return ps;
}
