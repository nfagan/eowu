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
  Mesh();
  ~Mesh() = default;
  
  void AddVertex(const eowu::Vertex &vertex);
  void SetIndices(const std::vector<eowu::u32>& indices);
  void SetTopology(eowu::u32 topology);
  
  void Draw();
  bool HasIndices() const;
  bool IsFinalized() const;
  
  void Dispose();
  
private:
  std::vector<eowu::Vertex> vertices;
  std::vector<eowu::u32> indices;
  
  eowu::u32 topology;
  eowu::MeshData mesh_data;
  eowu::u64 n_fragments;
  
  bool is_finalized;
  
  void finalize();
  static eowu::u32 get_gl_topology(eowu::u32 topology);
};
