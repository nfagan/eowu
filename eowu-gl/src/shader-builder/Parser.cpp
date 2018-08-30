//
//  ShaderBuilder.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Parser.hpp"
#include "Components.hpp"
#include <sstream>

std::string join(const std::vector<std::string>& components, const char *delim) {
  std::ostringstream joined;
  std::copy(components.begin(), components.end(), std::ostream_iterator<std::string>(joined, delim));
  
  return joined.str();
}

std::string eowu::parser::vertex(const eowu::schema::Vertex &schema) {
  std::vector<std::string> res;
  
  res.push_back(eowu::components::declare_version(schema.version));
  
  for (const auto &attr : schema.attributes) {
    res.push_back(eowu::components::declare_attribute(attr.location, attr.name, attr.type));
  }
  
  for (const auto &output : schema.outputs) {
    res.push_back("out " + eowu::components::declare_variable(output.name, output.type));
  }
  
  for (const auto &un : schema.uniforms) {
    res.push_back(eowu::components::declare_uniform(un.name, un.type));
  }
  
  res.push_back(eowu::components::begin_main());
  
  for (const auto &mn : schema.main) {
    res.push_back(mn);
  }
  
  res.push_back(eowu::components::close_brace());
  
  return join(res, "\n");
}

std::string eowu::parser::fragment(const eowu::schema::Fragment &schema) {
  std::vector<std::string> res;
  
  res.push_back(eowu::components::declare_version(schema.version));
  
  for (const auto &input : schema.inputs) {
    res.push_back("in " + eowu::components::declare_variable(input.name, input.type));
  }
  
  for (const auto &output : schema.outputs) {
    res.push_back("out " + eowu::components::declare_variable(output.name, output.type));
  }
  
  for (const auto &un : schema.uniforms) {
    res.push_back(eowu::components::declare_uniform(un.name, un.type));
  }
  
  res.push_back(eowu::components::begin_main());
  
  for (const auto &mn : schema.main) {
    res.push_back(mn);
  }
  
  res.push_back(eowu::components::close_brace());
  
  return join(res, "\n");
}
