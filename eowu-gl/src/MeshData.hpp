//
//  MeshData.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include "Vertex.hpp"
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
  
  std::vector<eowu::f32> get_interleaved_data(const std::vector<eowu::Vertex> &vertices) const;
  
  bool is_created;
};
