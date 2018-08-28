//
//  Mesh.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include "Vertex.hpp"
#include "MeshData.hpp"
#include <eowu-common/types.hpp>
#include <vector>

namespace eowu {
  class Mesh;
}

class eowu::Mesh {
public:
  Mesh() = default;
  ~Mesh() = default;
  
  void AddVertex(const eowu::Vertex &vertex);
  
private:
  std::vector<eowu::Vertex> vertices;
  std::vector<eowu::u32> indices;
  
  eowu::MeshData mesh_data;
};
