//
//  Vector.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>

namespace detail {
  
  template<unsigned int N>
  struct glm_vec;
  
  template<>
  struct glm_vec<2> {
    using type = glm::vec2;
    
    static inline type convert(const glm::vec2 &vec) {
      return vec;
    }
    
    static inline type convert(const glm::vec3 &vec) {
      return glm::vec2(vec);
    }
    
    static inline type convert(const glm::vec4 &vec) {
      return glm::vec2(vec);
    }
  };
  
  template<>
  struct glm_vec<3> {
    using type = glm::vec3;
    
    static inline type convert(const glm::vec2 &vec) {
      return glm::vec3(vec, 1.0f);
    }
    
    static inline type convert(const glm::vec3 &vec) {
      return vec;
    }
    
    static inline type convert(const glm::vec4 &vec) {
      return glm::vec3(vec);
    }
  };
  
  template<>
  struct glm_vec<4> {
    using type = glm::vec4;
    
    type convert(const glm::vec2 &vec) {
      return glm::vec4(vec, 1.0f, 1.0f);
    }
    
    type convert(const glm::vec3 &vec) {
      return glm::vec4(vec, 1.0f);
    }
    
    type convert(const glm::vec4 &vec) {
      return vec;
    }
  };
}

namespace eowu {
  template<unsigned int N>
  class Vector;
}

template<unsigned int N>
class eowu::Vector {
public:
  Vector(float val) {
    value = typename detail::glm_vec<N>::type(val);
  }
  
  Vector(const glm::vec2 &val) {
    value = detail::glm_vec<N>::convert(val);
  }
  
  Vector(const glm::vec3 &val) {
    value = detail::glm_vec<N>::convert(val);
  }
  
  Vector(const glm::vec4 &val) {
    value = detail::glm_vec<N>::convert(val);
  }
  
  const typename detail::glm_vec<N>::type& GetValue() const {
    return value;
  }
  
protected:
  typename detail::glm_vec<N>::type value;
};


