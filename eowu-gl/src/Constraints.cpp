//
//  Constraints.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#include "Constraints.hpp"
#include "Texture.hpp"

namespace detail {
  template<typename T>
  T numeric_zero_one(T val) {
    if (val < 0) {
      return 0;
    } else if (val > 1) {
      return 1;
    }
    
    return val;
  }
}

glm::vec3 eowu::constraints::zero_one(const glm::vec3 &val) {
  glm::vec3 res = val;
  
  res.x = detail::numeric_zero_one(val.x);
  res.y = detail::numeric_zero_one(val.y);
  res.z = detail::numeric_zero_one(val.z);
  
  return val;
}

eowu::Texture eowu::constraints::zero_one(const eowu::Texture &tex) {
  return tex;
}
