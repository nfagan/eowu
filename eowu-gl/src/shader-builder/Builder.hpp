//
//  Builder.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <string>
#include <memory>
#include <cstddef>
#include <vector>

namespace eowu {
  class Material;
  class Mesh;
  class Program;
  
  namespace schema {
    struct GLSLIdentifier;
    struct Fragment;
  }
  
  namespace builder {
    std::string vertex(const eowu::Mesh &mesh, const eowu::Material &material);
    std::string fragment(const eowu::Mesh &mesh, const eowu::Material &material);
    
    std::string vertex_text(const eowu::Material &material);
    std::string fragment_text(const eowu::Material &material);
    
    void configure_fragment_material(eowu::schema::Fragment &schema, const eowu::Material &material);
    
    std::shared_ptr<eowu::Program> from_source(const std::string &vertex, const std::string &fragment);
    
    bool has_identifier(const std::vector<eowu::schema::GLSLIdentifier> &identifiers, const char *name);
  }
}
