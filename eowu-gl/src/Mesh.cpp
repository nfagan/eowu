//
//  Mesh.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Mesh.hpp"
#include "Error.hpp"

void eowu::Mesh::AddVertex(const eowu::Vertex &vertex) {
  
  if (vertices.size() > 0) {
    const auto& v0 = vertices[0];
    
    if (!v0.Matches(vertex)) {
      throw eowu::NonMatchingVerticesError("Vertices must have consistent attributes.");
    }
  }
  
  vertices.push_back(vertex);
}
