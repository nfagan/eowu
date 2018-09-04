//
//  MeshFactory.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "MeshFactory.hpp"

void eowu::mesh_factory::make_quad(eowu::Mesh *mesh) {
  
  static const float vertex_data[48] = {
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
  };
  
  for (unsigned i = 0; i < 6; ++i) {
    eowu::Vertex vertex;
    
    float x = vertex_data[i*8+0];
    float y = vertex_data[i*8+1];
    float z = vertex_data[i*8+2];
    float u = vertex_data[i*8+3];
    float v = vertex_data[i*8+4];
    
    eowu::Attribute pos(eowu::attributes::position, {x, y, z});
    eowu::Attribute uv(eowu::attributes::uv, {u, v});
    
    vertex.AddAttribute(pos);
    vertex.AddAttribute(uv);
    
    mesh->AddVertex(vertex);
  }
  
  mesh->SetTopology(eowu::topologies::triangles);
}
