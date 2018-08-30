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

namespace eowu {
  class Material;
  class Mesh;
  class Program;
  
  
  namespace builder {
    std::string vertex(const eowu::Mesh &mesh, const eowu::Material &material);
    std::string fragment(const eowu::Mesh &mesh, const eowu::Material &material);
    
    std::shared_ptr<eowu::Program> from_source(const std::string &vertex, const std::string &fragment);    
  }
}
