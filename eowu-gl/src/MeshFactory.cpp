//
//  MeshFactory.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "MeshFactory.hpp"
#include <array>
#include <cmath>

void eowu::mesh_factory::make_frame_circle(eowu::Mesh *mesh) {
  static const float PI = 3.14159265359;
  static const float deg2rad = PI / 180.0f;
  
  for (unsigned int i = 0; i <= 360; i++) {
    eowu::Vertex vert;
    
    float rad = float(i) * deg2rad;
    
    float x = std::cos(rad) * 0.5f;
    float y = std::sin(rad) * 0.5f;
    
    eowu::Attribute pos(eowu::attributes::position, {x, y, 0.0f});
    eowu::Attribute uv(eowu::attributes::uv, {0.0f, 0.0f});
    
    vert.AddAttribute(pos);
    vert.AddAttribute(uv);
    
    mesh->AddVertex(vert);
  }
  
  mesh->SetTopology(eowu::topologies::line_strip);
}

void eowu::mesh_factory::make_frame_triangle(eowu::Mesh *mesh) {
  static const std::array<float, 8> vertex_data{
    -0.5f, -0.5f,
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };
  
  for (unsigned int i = 0; i < 4; i++) {
    eowu::Vertex vert;
    
    float x = vertex_data[i*2+0];
    float y = vertex_data[i*2+1];
    
    eowu::Attribute pos(eowu::attributes::position, {x, y, 0.0f});
    eowu::Attribute uv(eowu::attributes::uv, {0.0f, 0.0f});
    
    vert.AddAttribute(pos);
    vert.AddAttribute(uv);
    
    mesh->AddVertex(vert);
  }
  
  mesh->SetTopology(eowu::topologies::line_strip);
}

void eowu::mesh_factory::make_frame_quad(eowu::Mesh *mesh) {
  static const std::array<float, 16> vertex_data{
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    
    -0.5f, -0.5f,
    0.5f, -0.5f,
    
    0.5f, -0.5f,
    0.5f, 0.5f,
    
    0.5f, 0.5f,
    -0.5f, 0.5f
  };
  
  for (unsigned i = 0; i < 8; i++) {
    eowu::Vertex vertex;
    
    float x = vertex_data[i*2+0];
    float y = vertex_data[i*2+1];
    float z = 0.0f;
    float u = 0.0f;
    float v = 0.0f;
    
    eowu::Attribute pos(eowu::attributes::position, {x, y, z});
    eowu::Attribute uv(eowu::attributes::uv, {u, v});
    
    vertex.AddAttribute(pos);
    vertex.AddAttribute(uv);
    
    mesh->AddVertex(vertex);
  }
  
  mesh->SetTopology(eowu::topologies::lines);
}

void eowu::mesh_factory::make_quad(eowu::Mesh *mesh) {
  
  static const std::array<float, 48> vertex_data{
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
  };
  
  for (unsigned i = 0; i < 6; i++) {
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
  
  static const std::array<float, 24> vertex_data{
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
  };
  
  for (unsigned i = 0; i < 3; i++) {
    eowu::Vertex vertex;
    
    float x = vertex_data[i*8+0] * 0.5f;
    float y = vertex_data[i*8+1] * 0.5f;
    float z = vertex_data[i*8+2] * 0.5f;
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

void eowu::mesh_factory::make_default_sphere(eowu::Mesh *mesh) {
  make_sphere(mesh, 32);
}

void eowu::mesh_factory::make_sphere(Mesh *mesh, int vertex_count) {
  
  static const float PI = 3.14159265359;
  
  for (unsigned int i = 0; i<vertex_count; i++) {
    for (unsigned int j = 0; j<vertex_count; j++) {
      
      float x_segment = float(j)/float(vertex_count-1);
      float y_segment = float(i)/float(vertex_count-1);
      
      float x_pos = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI) * 0.5f;
      float y_pos = std::cos(y_segment * PI) * 0.5f;
      float z_pos = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI) * 0.5f;
      
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
