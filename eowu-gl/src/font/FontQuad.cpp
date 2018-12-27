//
//  FontQuad.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#include "FontQuad.hpp"
#include "FontGlyph.hpp"
#include <glad/glad.h>
#include <cstring>
#include <iostream>

//  https://learnopengl.com/In-Practice/Text-Rendering

eowu::FontQuad::FontQuad() : vao(0), vbo(0), is_created(false) {
  std::memset(&vertices[0], 0.0f, vertices.size() * sizeof(float));
  assign_texture_coordinates();
}

float eowu::FontQuad::Configure(const eowu::FontGlyph &glyph, const glm::vec2 &position, float scale) {
  if (!glyph.IsCreated()) {
    return 0.0f;
  }
  
  const auto &size = glyph.GetSize();
  const auto &bearing = glyph.GetBearing();
  const auto advance = glyph.GetAdvance();
  
  float x = position.x + bearing.x * scale;
  float y = position.y - (size.y - bearing.y) * scale;
  
  float w = size.x * scale;
  float h = size.y * scale;
  
  assign_vertex_data(x, y, w, h);
  
  float next_x = float(advance >> 6) * scale;
  
  return next_x;
}

void eowu::FontQuad::assign_texture_coordinates() {
  float zero = 0.0f;
  float one = 1.0f;
  //  1.
  vertices[2] = zero;
  vertices[3] = zero;
  //  2.
  vertices[6] = zero;
  vertices[7] = one;
  //  3.
  vertices[10] = one;
  vertices[11] = one;
  //  4.
  vertices[14] = zero;
  vertices[15] = zero;
  //  5.
  vertices[18] = one;
  vertices[19] = one;
  //  6.
  vertices[22] = one;
  vertices[23] = zero;
}

void eowu::FontQuad::assign_vertex_data(float x, float y, float w, float h) {
  //  1.
  vertices[0] = x;
  vertices[1] = y + h;
  //  2.
  vertices[4] = x;
  vertices[5] = y;
  //  3.
  vertices[8] = x + w;
  vertices[9] = y;
  //  4.
  vertices[12] = x;
  vertices[13] = y + h;
  //  5.
  vertices[16] = x + w;
  vertices[17] = y;
  //  6.
  vertices[20] = x + w;
  vertices[21] = y + h;
}

bool eowu::FontQuad::IsCreated() const {
  return is_created;
}

void eowu::FontQuad::Create() {
  if (IsCreated()) {
    Dispose();
  }
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  is_created = true;
}

void eowu::FontQuad::Dispose() const {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void eowu::FontQuad::Bind() const {
  glBindVertexArray(vao);
}

void eowu::FontQuad::Unbind() const {
  glBindVertexArray(0);
}

void eowu::FontQuad::Draw() const {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
