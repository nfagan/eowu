//
//  Constraints.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <glm/glm.hpp>

namespace eowu {
  class Texture;
  
  namespace constraints {
    glm::vec3 zero_one(const glm::vec3 &val);
    eowu::Texture zero_one(const eowu::Texture &val);
  }
}
