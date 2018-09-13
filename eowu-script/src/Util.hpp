//
//  Util.hpp
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace eowu {
  namespace util {
    template<typename T>
    glm::vec3 require_vec3(const std::vector<T> &other);
  }
}

template<typename T>
glm::vec3 eowu::util::require_vec3(const std::vector<T> &other) {
  std::size_t n_els = other.size();
  glm::vec3 result;
  
  for (std::size_t i = 0; i < 3; i++) {
    result[i] = n_els <= i ? 0 : other[i];
  }
  
  return result;
}
