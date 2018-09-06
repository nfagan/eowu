//
//  MeshLibrary.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Vertex.hpp"
#include "Attribute.hpp"
#include "AttributeTypes.hpp"
#include "Mesh.hpp"
#include "Topologies.hpp"

namespace eowu {
  namespace mesh_factory {
    void make_quad(eowu::Mesh *mesh);
    void make_triangle(eowu::Mesh *mesh);
    void make_sphere(eowu::Mesh *mesh, int vertex_count = 64);
  }
}
