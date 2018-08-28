//
//  MeshData.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "MeshData.hpp"
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
  
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);
  
  if (vertices.size() == 0) {
    return;
  }
  
  Bind();
  
  is_created = true;
}

bool eowu::MeshData::IsCreated() const {
  return is_created;
}

std::vector<eowu::f32> eowu::MeshData::get_interleaved_data(const std::vector<eowu::Vertex> &vertices) const {
  
  std::vector<eowu::f32> res;
  
  if (vertices.size() == 0) {
    return res;
  } 
  
  const auto attrib_kinds = vertices[0].GetAttributeKinds();
  
  for (const auto& vert : vertices) {
    for (const auto& kind : attrib_kinds) {
      
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
