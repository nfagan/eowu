//
//  Util.hpp
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cstddef>

namespace eowu {
  namespace util {
    template<typename T>
    glm::vec3 require_vec3(const std::vector<T> &other);
    
    template<typename T>
    glm::vec2 require_vec2(const std::vector<T> &other);
    
    namespace priv {
      template<typename Vec, typename T>
      Vec require_vector(const std::vector<T> &other, std::size_t N);
    }
  }
}

template<typename Vec, typename T>
Vec eowu::util::priv::require_vector(const std::vector<T> &other, std::size_t N) {
  std::size_t n_els = other.size();
  Vec result;
  
  for (std::size_t i = 0; i < N; i++) {
    result[i] = n_els <= i ? 0 : other[i];
  }
  
  return result;
}

template<typename T>
glm::vec3 eowu::util::require_vec3(const std::vector<T> &other) {
  return priv::require_vector<glm::vec3>(other, 3);
}

template<typename T>
glm::vec2 eowu::util::require_vec2(const std::vector<T> &other) {
  return priv::require_vector<glm::vec2>(other, 2);
}
