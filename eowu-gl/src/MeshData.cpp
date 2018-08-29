//
//  MeshData.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "MeshData.hpp"
#include "Attribute.hpp"
#include "AttributeTypes.hpp"
#include <glad/glad.h>

eowu::MeshData::MeshData() {
  is_created = false;
}

eowu::MeshData::~MeshData() {
  Dispose();
}

void eowu::MeshData::Dispose() {
  if (!IsCreated()) {
    return;
  }
  
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  
  is_created = false;
}

void eowu::MeshData::Bind() const {
  glBindVertexArray(vao);
}

void eowu::MeshData::Unbind() const {
  glBindVertexArray(0);
}

void eowu::MeshData::Create(const std::vector<eowu::Vertex> &vertices, const std::vector<eowu::u32> &indices) {
  if (IsCreated()) {
    Dispose();
  }
  
  create_components();
  
  if (vertices.size() == 0) {
    return;
  }
  
  Bind();
  
  create_attributes(vertices);
  create_indices(indices);
  
  Unbind();
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  is_created = true;
}

void eowu::MeshData::create_indices(const std::vector<eowu::u32> &indices) {
  eowu::u64 sz = indices.size();
  
  if (sz == 0) {
    return;
  }
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eowu::u32) * sz, &indices[0], GL_STATIC_DRAW);
}

void eowu::MeshData::create_attributes(const std::vector<eowu::Vertex> &vertices) {
  const auto& v0 = vertices[0];
  
  auto interleaved_data = get_interleaved_data(vertices);
  auto float_size = sizeof(eowu::AttributeComponentType);
  auto data_size = interleaved_data.size() * float_size;
  
  if (data_size == 0) {
    return;
  }
  
  GLsizei stride = float_size * v0.Size();
  eowu::u32 offset = 0;
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data_size, &interleaved_data[0], GL_STATIC_DRAW);
  
  auto attrib_kinds = get_ordered_attribute_kinds();
  eowu::u64 idx = 0;
  
  for (const auto& kind : attrib_kinds) {
    if (!v0.HasAttribute(kind)) {
      continue;
    }
    
    auto sz = v0.GetAttribute(kind)->Size();
    
    glVertexAttribPointer(idx, sz, GL_FLOAT, GL_FALSE, stride, (void*)(offset*float_size));
    glEnableVertexAttribArray(idx);
    
    offset += sz;
    
    idx++;
  }
}

void eowu::MeshData::create_components() {
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);
}

bool eowu::MeshData::IsCreated() const {
  return is_created;
}

std::vector<const char*> eowu::MeshData::get_ordered_attribute_kinds() {
  std::vector<const char*> res;
  
  res.push_back(eowu::attributes::position);
  res.push_back(eowu::attributes::uv);
  
  return res;
}

eowu::AttributeAggregateType eowu::MeshData::get_interleaved_data(const std::vector<eowu::Vertex> &vertices) const {
  
  eowu::AttributeAggregateType res;
  
  if (vertices.size() == 0) {
    return res;
  }
  
  const auto attrib_kinds = get_ordered_attribute_kinds();
  
  for (const auto& vert : vertices) {
    for (const auto& kind : attrib_kinds) {
      
      if (!vert.HasAttribute(kind)) {
        continue;
      }
      
      const auto* attrib = vert.GetAttribute(kind);
      const auto& components = attrib->GetComponents();
      const eowu::u64 sz = attrib->Size();
      
      for (eowu::u64 i = 0; i < sz; i++) {
        res.push_back(components[i]);
      }
    }
  }
  
  return res;
}
