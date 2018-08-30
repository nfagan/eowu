//
//  Components.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Components.hpp"

std::string eowu::components::declare_variable(const std::string &name, eowu::glsl::Types type) {
  return eowu::glsl::get_string_type(type) + " " + name + ";";
}

std::string eowu::components::declare_version(const std::string &version) {
  return "#version " + version;
}

std::string eowu::components::declare_attribute(const std::string &loc, const std::string &name, eowu::glsl::Types type) {
  return "layout (location = " + loc + ") in " + declare_variable(name, type);
}

std::string eowu::components::declare_uniform(const std::string &name, eowu::glsl::Types type) {
  return "uniform " + declare_variable(name, type);
}

std::string eowu::components::to_vec3(const std::string &name, eowu::glsl::Types type) {
  switch (type) {
    case glsl::sampler2D:
      return "texture(" + name + ", tx).rgb";
    case glsl::vec3:
      return name;
    default:
      return "vec3(" + name + ")";
  }
}

std::string eowu::components::begin_main() {
  return "void main() {";
}

std::string eowu::components::open_brace() {
  return "{";
}

std::string eowu::components::close_brace() {
  return "}";
}
