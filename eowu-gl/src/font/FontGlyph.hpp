//
//  FontGlyph.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/23/18.
//

#pragma once

#include "FontPlatform.hpp"
#include "../Texture.hpp"
#include <glm/glm.hpp>

namespace eowu {
  class FontGlyph;
}

class eowu::FontGlyph {
public:
  FontGlyph();
  ~FontGlyph() = default;
  
  void Dispose() const;
  bool IsCreated() const;
  const glm::ivec2& GetSize() const;
  const glm::ivec2& GetBearing() const;
  const eowu::Texture& GetTexture() const;
  unsigned int GetAdvance() const;
  void Bind() const;
  
  static eowu::FontGlyph FromFontFace(FT_Face font_face);
private:
  bool is_created;
  
  eowu::Texture glyph_texture;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};
