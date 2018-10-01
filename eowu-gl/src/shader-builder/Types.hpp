//
//  Types.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace eowu {
  class Texture;
  
  namespace glsl {
    enum Types {
      vec2,
      vec3,
      vec4,
      mat4,
      sampler2D,
      float_t
    };
    
    std::string get_string_type(eowu::glsl::Types type);
    
    //
    //  glsl type -> string glsl type label
    //
    
    template<eowu::glsl::Types type>
    struct StringTypeMap;
    
    template<>
    struct StringTypeMap<eowu::glsl::vec2> {
      static const char *value;
    };
    
    template<>
    struct StringTypeMap<eowu::glsl::vec3> {
      static const char *value;
    };
    
    template<>
    struct StringTypeMap<eowu::glsl::vec4> {
      static const char *value;
    };
    
    template<>
    struct StringTypeMap<eowu::glsl::mat4> {
      static const char *value;
    };
    
    template<>
    struct StringTypeMap<eowu::glsl::sampler2D> {
      static const char *value;
    };
    
    //
    //  c++ type -> glsl type id
    //
    
    template<class T>
    struct TypeMap {};
    
    template<>
    struct TypeMap<glm::vec3> {
      static constexpr eowu::glsl::Types value = eowu::glsl::vec3;
    };
    
    template<>
    struct TypeMap<eowu::Texture> {
      static constexpr eowu::glsl::Types value = eowu::glsl::sampler2D;
    };
    
    template<>
    struct TypeMap<float> {
      static constexpr eowu::glsl::Types value = eowu::glsl::float_t;
    };
  }
  
  namespace schema {
    struct GLSLIdentifier {
      std::string name;
      eowu::glsl::Types type;
    };
    
    struct GLSLAttribute {
      std::string location;
      std::string name;
      eowu::glsl::Types type;
    };
    
    struct Fragment {
      std::string version;
      
      std::vector<eowu::schema::GLSLIdentifier> uniforms;
      std::vector<eowu::schema::GLSLIdentifier> inputs;
      std::vector<eowu::schema::GLSLIdentifier> outputs;
      
      std::vector<std::string> main;
    };
    
    struct Vertex {
      std::string version;
      
      std::vector<eowu::schema::GLSLAttribute> attributes;
      std::vector<eowu::schema::GLSLIdentifier> uniforms;
      std::vector<eowu::schema::GLSLIdentifier> outputs;
      
      std::vector<std::string> main;
    };
  }
}
