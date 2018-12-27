//
//  FontGlyph.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/23/18.
//

#include "FontGlyph.hpp"
#include <glad/glad.h>
#include <iostream>

eowu::FontGlyph::FontGlyph() :
is_created(false), size(0, 0), bearing(0, 0), advance(0) {
  //
}

void eowu::FontGlyph::Dispose() const {
  if (!is_created) {
    return;
  }
  
  glyph_texture.Dispose();
}

bool eowu::FontGlyph::IsCreated() const {
  return is_created;
}

const glm::ivec2& eowu::FontGlyph::GetSize() const {
  return size;
}

const glm::ivec2& eowu::FontGlyph::GetBearing() const {
  return bearing;
}

const eowu::Texture& eowu::FontGlyph::GetTexture() const {
  return glyph_texture;
}

unsigned int eowu::FontGlyph::GetAdvance() const {
  return advance;
}

void eowu::FontGlyph::Bind() const {
  if (!IsCreated()) {
    return;
  }
  
  glyph_texture.Bind();
}

eowu::FontGlyph eowu::FontGlyph::FromFontFace(FT_Face font_face) {
  //  https://learnopengl.com/In-Practice/Text-Rendering
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
  GLuint texture_handle;
  
  auto glyph = font_face->glyph;
  auto bitmap = glyph->bitmap;
  
  auto width = bitmap.width;
  auto rows = bitmap.rows;
  void *data = bitmap.buffer;
  
  auto left = glyph->bitmap_left;
  auto top = glyph->bitmap_top;
  auto advance = glyph->advance.x;
  
  glGenTextures(1, &texture_handle);
  glBindTexture(GL_TEXTURE_2D, texture_handle);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, rows, 0, GL_RED, GL_UNSIGNED_BYTE, data);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  eowu::Texture texture;
  
  texture.SetId(texture_handle);
  texture.SetWidth(width);
  texture.SetHeight(rows);
  
  eowu::FontGlyph font_glyph;
  
  font_glyph.glyph_texture = texture;
  font_glyph.size = glm::ivec2(width, rows);
  font_glyph.bearing = glm::ivec2(left, top);
  font_glyph.advance = advance;
  font_glyph.is_created = true;
  
  return font_glyph;
}
