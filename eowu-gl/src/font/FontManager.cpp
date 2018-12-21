//
//  FontManager.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/28/18.
//

#include "FontManager.hpp"
#include "FontFace.hpp"
#include "../Error.hpp"

eowu::FontManager::FontManager() :
is_initialized(false) {
  //
}

eowu::FontManager::~FontManager() {
  FT_Done_FreeType(ft_library);
}

void eowu::FontManager::Initialize() {
  if (is_initialized) {
    return;
  }
  
  auto res = FT_Init_FreeType(&ft_library);
  
  if (res) {
    throw eowu::FontError("Failed to initialize FreeType library.");
  }
  
  is_initialized = true;
}

std::shared_ptr<eowu::FontFace> eowu::FontManager::GetFontFace(const std::string &filename, eowu::s32 face_index) {
  auto face = std::make_shared<eowu::FontFace>(this);
  
  face->open(filename.c_str(), face_index);
  
  return face;
}

void eowu::FontManager::assert_initialized() {
  if (!is_initialized) {
    throw eowu::FontError("FontManager is not yet initialized.");
  }
}
