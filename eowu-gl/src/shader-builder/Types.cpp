//
//  Types.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Types.hpp"

const char *eowu::glsl::StringTypeMap<eowu::glsl::vec2>::value = "vec2";
const char *eowu::glsl::StringTypeMap<eowu::glsl::vec3>::value = "vec3";
const char *eowu::glsl::StringTypeMap<eowu::glsl::vec4>::value = "vec4";
const char *eowu::glsl::StringTypeMap<eowu::glsl::mat4>::value = "mat4";
const char *eowu::glsl::StringTypeMap<eowu::glsl::sampler2D>::value = "sampler2D";

std::string eowu::glsl::get_string_type(eowu::glsl::Types type) {
  switch (type) {
    case vec2:
      return "vec2";
    case vec3:
      return "vec3";
    case vec4:
      return "vec4";
    case mat4:
      return "mat4";
    case sampler2D:
      return "sampler2D";
    default:
      return "";
  }
}

