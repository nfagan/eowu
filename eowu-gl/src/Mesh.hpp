//
//  Mesh.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include "Vertex.hpp"
#include "MeshData.hpp"
#include "Identifier.hpp"
#include <eowu-common/types.hpp>
#include <vector>
#include <unordered_map>

namespace eowu {
  class Mesh;
}

class eowu::Mesh {
public:
  Mesh() = default;
  ~Mesh();
  
  void AddVertex(const eowu::Vertex &vertex);
  void SetIndices(const std::vector<eowu::u32>& indices);
  void SetTopology(eowu::u32 topology);
  
  void Bind(const eowu::Identifier &window_id);
  void Unbind(const eowu::Identifier &window_id);
  void Draw() const;
  bool HasIndices() const;
  bool HasAttribute(const std::string &attr) const;
  bool IsFinalized(const eowu::Identifier &window_id) const;
  const eowu::Identifier& GetIdentifier() const;
  
  void Dispose();  
private:
  std::vector<eowu::Vertex> vertices;
  std::vector<eowu::u32> indices;
  
  eowu::Identifier resource_id;
  
  eowu::u32 topology;
  
  std::unordered_map<eowu::Identifier, eowu::MeshData> mesh_data;
  std::unordered_map<eowu::Identifier, bool> is_finalized;
  
  eowu::u64 n_fragments;
  
  void finalize(const eowu::Identifier &window_id);
  static eowu::u32 get_gl_topology(eowu::u32 topology);
};
