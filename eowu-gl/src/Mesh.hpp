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
#include <cstddef>

namespace eowu {
  class Mesh;
}

class eowu::Mesh {
public:
  Mesh() = default;
  ~Mesh();
  
  void AddVertex(const eowu::Vertex &vertex);
  void SetIndices(const std::vector<unsigned int>& indices);
  void SetTopology(unsigned int topology);
  
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
  std::vector<unsigned int> indices;
  
  eowu::Identifier resource_id;
  
  unsigned int topology;
  
  std::unordered_map<eowu::Identifier, eowu::MeshData> mesh_data;
  std::unordered_map<eowu::Identifier, bool> is_finalized;
  
  std::size_t n_fragments;
  
  void finalize(const eowu::Identifier &window_id);
  static unsigned int get_gl_topology(unsigned int topology);
};
