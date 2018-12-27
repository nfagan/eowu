//
//  FontQuad.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#pragma once

#include <glm/glm.hpp>
#include <array>

namespace eowu {
  class FontQuad;
  class FontGlyph;
}

class eowu::FontQuad {
public:
  FontQuad();
  ~FontQuad() = default;
  
  float Configure(const eowu::FontGlyph &glyph, const glm::vec2 &position, float scale);
  void Dispose() const;
  void Create();
  bool IsCreated() const;
  
  void Bind() const;
  void Draw() const;
  void Unbind() const;
  
private:
  std::array<float, 24> vertices;
  
  unsigned int vao;
  unsigned int vbo;
  
  bool is_created;
private:
  
  void assign_texture_coordinates();
  void assign_vertex_data(float x, float y, float w, float h);
};
