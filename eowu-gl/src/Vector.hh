//
//  Vector.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>

namespace detail {
  
  template<eowu::u32 N>
  struct glm_vec;
  
  template<>
  struct glm_vec<2> {
    using type = glm::vec2;
    
    static inline glm::vec2 create(const glm::vec2 &vec) {
      return vec;
    }
    
    static inline glm::vec2 create(const glm::vec3 &vec) {
      return glm::vec2(vec.x, vec.y);
    }
    
    static inline glm::vec2 create(const glm::vec4 &vec) {
      return glm::vec2(vec.x, vec.y);
    }
  };
  
  template<>
  struct glm_vec<3> {
    using type = glm::vec3;
    
    static inline glm::vec3 create(const glm::vec2 &vec) {
      return glm::vec3(vec.x, vec.y, 1.0f);
    }
    
    static inline glm::vec3 create(const glm::vec3 &vec) {
      return vec;
    }
    
    static inline glm::vec3 create(const glm::vec4 &vec) {
      return glm::vec3(vec.x, vec.y, vec.z);
    }
  };
  
  template<>
  struct glm_vec<4> {
    using type = glm::vec4;
    
    static inline glm::vec4 create(const glm::vec2 &vec) {
      return glm::vec4(vec.x, vec.y, 1.0f, 1.0f);
    }
    
    static inline glm::vec4 create(const glm::vec3 &vec) {
      return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
    }
    
    static inline glm::vec4 create(const glm::vec4 &vec) {
      return vec;
    }
  };
  
  //  float -> vector
  template <typename T, eowu::u32 N>
  inline typename std::enable_if<std::is_same<T, std::decay<eowu::f32>::type>::value, typename glm_vec<N>::type>::type
  create(const T &val) {
    return typename glm_vec<N>::type(val);
  }
  
  //  vector -> vector
  template <typename T, eowu::u32 N>
  inline typename std::enable_if<std::is_same<T, std::decay<glm::vec3>::type>::value, typename glm_vec<N>::type>::type
  create(const T &val) {
    return glm_vec<N>::create(val);
  }
  
  
}

namespace eowu {
  template<class T, eowu::u32 N = 3>
  class Vector;
  
  template<class T = eowu::f32>
  class Vec2 : public eowu::Vector<T, 2> {
  public:
    Vec2(const T &val) : eowu::Vector<T, 2>(val) {};
  };
  
  template<class T = eowu::f32>
  class Vec3 : public eowu::Vector<T, 3> {
  public:
    Vec3(const T &val) : eowu::Vector<T, 3>(val) {};
  };
  
  template<class T = eowu::f32>
  class Vec4 : public eowu::Vector<T, 4> {
  public:
    Vec4(const T &val) : eowu::Vector<T, 4>(val) {};
  };
}

template<class T, eowu::u32 N>
class eowu::Vector {
public:
  Vector(const T &val) {
    value = detail::create<T, N>(val);
  }
  
  const T& GetValue() {
    return value;
  }
  
protected:
  typename detail::glm_vec<N>::type value;
};


