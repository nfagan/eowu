//
//  MeshData.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include "Vertex.hpp"
#include <eowu-common/types.hpp>
#include <vector>

namespace eowu {
  class MeshData;
}

class eowu::MeshData {
  
public:
  MeshData();
  ~MeshData();
  
  void Create(const std::vector<eowu::Vertex> &vertices, const std::vector<eowu::u32> &indices);
  bool IsCreated() const;
  void Dispose();
  void Bind() const;
  void Unbind() const;
  
private:
  eowu::u32 vbo;
  eowu::u32 vao;
  eowu::u32 ebo;
  
  bool is_created;
  
  void create_components();
  void create_attributes(const std::vector<eowu::Vertex> &vertices);
  void create_indices(const std::vector<eowu::u32> &indices);
  
  std::vector<eowu::f32> get_interleaved_data(const std::vector<eowu::Vertex> &vertices) const;
  static std::vector<const char*> get_ordered_attribute_kinds();
};
