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
  
  void Create(const std::vector<eowu::Vertex> &vertices, const std::vector<unsigned int> &indices);
  bool IsCreated() const;
  void Dispose();
  void Bind() const;
  void Unbind() const;
  
private:
  unsigned int vbo;
  unsigned int vao;
  unsigned int ebo;
  
  bool is_created;
  
  void create_components();
  void create_attributes(const std::vector<eowu::Vertex> &vertices);
  void create_indices(const std::vector<unsigned int> &indices);
  
  std::vector<float> get_interleaved_data(const std::vector<eowu::Vertex> &vertices) const;
  static std::vector<const char*> get_ordered_attribute_kinds();
};
