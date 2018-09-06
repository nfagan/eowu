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
#include <eowu-common/logging.hpp>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

eowu::Mesh::~Mesh() {
  for (auto &data : mesh_data) {
    data.second.Dispose();
  }
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

void eowu::Mesh::SetIndices(const std::vector<unsigned int> &indices) {
  this->indices = indices;
}

void eowu::Mesh::SetTopology(unsigned int topology) {
  this->topology = topology;
}

const eowu::Identifier& eowu::Mesh::GetIdentifier() const {
  return resource_id;
}

bool eowu::Mesh::IsFinalized(const eowu::Identifier &id) const {
  return is_finalized.count(id) > 0 && is_finalized.at(id);
}

bool eowu::Mesh::HasIndices() const {
  return indices.size() > 0;
}

bool eowu::Mesh::HasAttribute(const std::string &attr) const {
  if (vertices.size() == 0) {
    return false;
  }
  
  return vertices[0].HasAttribute(attr);
}

void eowu::Mesh::Bind(const eowu::Identifier &window_id) {
  if (!IsFinalized(window_id)) {
    finalize(window_id);
  }
  
  mesh_data.at(window_id).Bind();
}

void eowu::Mesh::Unbind(const eowu::Identifier &window_id) {
  if (!IsFinalized(window_id)) {
    finalize(window_id);
  }
  
  mesh_data.at(window_id).Unbind();
}

void eowu::Mesh::Draw() const {
  auto gl_topology = get_gl_topology(topology);
  
  if (HasIndices()) {
    glDrawElements(gl_topology, n_fragments, GL_UNSIGNED_INT, 0);
  } else {
    glDrawArrays(gl_topology, 0, n_fragments);
  }
}

void eowu::Mesh::finalize(const eowu::Identifier &window_id) {
  if (IsFinalized(window_id)) {
    return;
  }
  
  eowu::MeshData tmp_mesh_data;
  tmp_mesh_data.Create(vertices, indices);
  
  if (HasIndices()) {
    n_fragments = indices.size();
  } else {
    n_fragments = vertices.size();
  }
  
  mesh_data[window_id] = tmp_mesh_data;
  is_finalized[window_id] = true;
}

unsigned int eowu::Mesh::get_gl_topology(unsigned int topology) {
  switch (topology) {
    case eowu::topologies::triangles:
      return GL_TRIANGLES;
    case eowu::topologies::triangle_strip:
      return GL_TRIANGLE_STRIP;
    default:
      EOWU_LOG_WARN("Mesh::get_gl_topology: Unrecognized topology. Using 'GL_TRIANGLES'.");
      return GL_TRIANGLES;
  }
}
