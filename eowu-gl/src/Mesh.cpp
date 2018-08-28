//
//  Mesh.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Mesh.hpp"
#include "Error.hpp"
#include "Topologies.hpp"
#include <glad/glad.h>

eowu::Mesh::Mesh() {
  is_finalized = false;
}

void eowu::Mesh::AddVertex(const eowu::Vertex &vertex) {
  
  if (vertices.size() > 0) {
    const auto& v0 = vertices[0];
    
    if (!v0.Matches(vertex)) {
      throw eowu::NonMatchingVerticesError("Vertices must have consistent attributes.");
    }
  }
  
  vertices.push_back(vertex);
}

void eowu::Mesh::SetIndices(const std::vector<eowu::u32> &indices) {
  this->indices = indices;
}

void eowu::Mesh::SetTopology(eowu::u32 topology) {
  this->topology = topology;
}

bool eowu::Mesh::IsFinalized() const {
  return is_finalized;
}

bool eowu::Mesh::HasIndices() const {
  return indices.size() > 0;
}

void eowu::Mesh::Draw() {
  if (!IsFinalized()) {
    finalize();
  }
  
  mesh_data.Bind();
  
  auto gl_topology = get_gl_topology(topology);
  
  if (HasIndices()) {
    glDrawElements(gl_topology, n_fragments, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(gl_topology, 0, n_fragments);
  }
  
  mesh_data.Unbind();
}

void eowu::Mesh::finalize() {
  if (IsFinalized()) {
    return;
  }
  
  mesh_data.Create(vertices, indices);
  
  if (HasIndices()) {
    n_fragments = indices.size();
  } else {
    n_fragments = vertices.size();
  }
  
  is_finalized = true;
}

eowu::u32 eowu::Mesh::get_gl_topology(eowu::u32 topology) {
  switch (topology) {
    case eowu::topologies::triangles:
      return GL_TRIANGLES;
    case eowu::topologies::triangle_strip:
      return GL_TRIANGLE_STRIP;
    default:
      return GL_TRIANGLES;
  }
}
