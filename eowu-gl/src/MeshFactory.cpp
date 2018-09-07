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

void eowu::mesh_factory::make_triangle(eowu::Mesh *mesh) {
  
  static const float vertex_data[24] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
  };
  
  for (unsigned i = 0; i < 3; ++i) {
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

void eowu::mesh_factory::make_sphere(Mesh *mesh, int vertex_count) {
  
  static const float PI = 3.14159265359;
  
  for (unsigned int i = 0; i<vertex_count; i++) {
    for (unsigned int j = 0; j<vertex_count; j++) {
      
      float x_segment = float(j)/float(vertex_count-1);
      float y_segment = float(i)/float(vertex_count-1);
      
      float x_pos = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      float y_pos = std::cos(y_segment * PI);
      float z_pos = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
      
      eowu::Vertex vertex;
      
      eowu::Attribute pos(eowu::attributes::position, {x_pos, y_pos, z_pos});
      eowu::Attribute uv(eowu::attributes::uv, {x_segment, y_segment});
      
      vertex.AddAttribute(pos);
      vertex.AddAttribute(uv);
      
      mesh->AddVertex(vertex);
    }
  }
  
  unsigned int first_index = 0;
  unsigned int next_index = first_index + vertex_count;
  std::vector<unsigned int> indices;
  unsigned int index_stp = 0;
  bool should_proceed = true;
  
  while (should_proceed) {
    indices.push_back(first_index);
    indices.push_back(next_index);
    index_stp += 2;
    
    should_proceed = next_index != ((vertex_count * vertex_count) - 1);
    
    if (index_stp > 0 && (next_index+1) % vertex_count == 0 && should_proceed) {
      indices.push_back(next_index);
      indices.push_back(first_index+1);
      index_stp += 2;
    }
    
    first_index++;
    next_index++;
  }
  
  mesh->SetTopology(eowu::topologies::triangle_strip);
  mesh->SetIndices(indices);
}
