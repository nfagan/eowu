//
//  FontFace.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/28/18.
//

#include "FontFace.hpp"
#include "FontManager.hpp"
#include "FontGlyph.hpp"
#include "../Error.hpp"
#include <eowu-common/unicode.hpp>
#include <cstddef>
#include <iostream>

eowu::FontFace::FontFace(eowu::FontManager *manager) :
font_manager(manager), is_initialized(false), is_deleted(false) {
  //
}

eowu::FontFace::~FontFace() {
  dispose();
}

void eowu::FontFace::SetPixelSizes(eowu::FontFace::PixelSizes pixel_sizes) {
  assert_initialized();
  
  auto res = FT_Set_Pixel_Sizes(font_face, pixel_sizes.width, pixel_sizes.height);
  
  if (res) {
    throw eowu::FontError("Failed to set pixel sizes for font face", res);
  }
}

const eowu::FontGlyph& eowu::FontFace::GetGlyph(char character) const {
  std::string single_char(1, 'a');
  single_char[0] = character;
  
  return GetGlyph(single_char);
}

const eowu::FontGlyph& eowu::FontFace::GetGlyph(const std::string &character) const {
  auto it = glyphs.find(character);
  
  if (it == glyphs.end()) {
    throw eowu::FontError("Character: " + character + " does not have an associated glyph.");
  }
  
  return it->second;
}

bool eowu::FontFace::HasGlyph(const std::string &character) const {
  return glyphs.count(character) > 0;
}

bool eowu::FontFace::HasGlyph(char character) const {
  std::string single_char(1, 'a');
  single_char[0] = character;
  
  return HasGlyph(single_char);
}

void eowu::FontFace::open(const std::string &file, eowu::s32 face_index) {
  if (is_initialized) {
    throw eowu::FontError("Font is already initialized.");
  }
  
  auto res = FT_New_Face(font_manager->ft_library, file.c_str(), face_index, &font_face);
  
  if (res) {
    if (res == 1) {
      throw eowu::FontError("Failed to initialize font face: '" + file + "': File does not exist.");
    } else {
      throw eowu::FontError("Failed to initialize font face: " + file, res);
    }
  }
  
  is_initialized = true;
}

void eowu::FontFace::LoadGlyphsAscii() {
  assert_initialized();
  
  FT_Select_Charmap(font_face, ft_encoding_unicode);
  
  std::string character(1, 'a');
  
  for (unsigned int i = 0; i < 128; i++) {
    character[0] = char(i);
    
    auto res = FT_Load_Char(font_face, i, FT_LOAD_RENDER);
    
    if (res) {
      throw eowu::FontError("Failed to load character: " + character, res);
    }
    
    glyphs.emplace(character, FontGlyph::FromFontFace(font_face));
  }
}

//  @TODO: Figure out unicode mapping issue.
//    This method is private for now.
void eowu::FontFace::LoadGlyphs(const std::string &characters) {
  assert_initialized();
  
  //  Use u32 representation
  auto utf_characters = eowu::unicode::u8_to_u32(characters);
  std::size_t iterator_index = 0;
  
  FT_Select_Charmap(font_face, ft_encoding_unicode);
  
  std::string character(1, 'a');
  
  for (const auto &it : utf_characters) {
    auto glyph_index = FT_Get_Char_Index(font_face, it);
    character[0] = characters.at(iterator_index);
    
    if (glyphs.count(character) > 0) {
      continue;
    }
    
    auto res = FT_Load_Char(font_face, glyph_index, FT_LOAD_RENDER);
    
    if (res) {
      throw eowu::FontError("Failed to load character: " + character, res);
    }
    
    auto glyph = eowu::FontGlyph::FromFontFace(font_face);
    
    glyphs.emplace(character, glyph);
    
    iterator_index++;
  }
}

void eowu::FontFace::dispose() {
  if (is_deleted || !is_initialized) {
    return;
  }
  
  FT_Done_Face(font_face);
  
  for (const auto &it : glyphs) {
    it.second.Dispose();
  }
  
  is_deleted = true;
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
