//
//  Builder.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Builder.hpp"
#include "Components.hpp"
#include "Parser.hpp"
#include "Types.hpp"
#include "../Model.hpp"
#include "../Material.hpp"
#include "../AttributeTypes.hpp"
#include "../UniformNames.hpp"
#include "../Error.hpp"
#include "../Shader.hpp"
#include "../ShaderTypes.hpp"

std::string eowu::builder::vertex(const eowu::Mesh &mesh, const eowu::Material &material) {
  eowu::schema::Vertex v_schema;
  
  v_schema.version = "330 core";
  
  v_schema.uniforms.push_back({eowu::uniforms::model, eowu::glsl::mat4});
  v_schema.uniforms.push_back({eowu::uniforms::view, eowu::glsl::mat4});
  v_schema.uniforms.push_back({eowu::uniforms::projection, eowu::glsl::mat4});
  
  if (mesh.HasAttribute(eowu::attributes::position)) {
    v_schema.attributes.push_back({"0", eowu::attributes::position, eowu::glsl::vec3});
  }

  if (mesh.HasAttribute(eowu::attributes::uv)) {
    v_schema.attributes.push_back({"1", eowu::attributes::uv, eowu::glsl::vec2});
    
    v_schema.outputs.push_back({"tx", eowu::glsl::vec2});
    
    v_schema.main.push_back("tx = uv;");
  }
  
  v_schema.main.push_back("gl_Position = projection * view * model * vec4(position, 1.0f);");
  
  return eowu::parser::vertex(v_schema);
}

std::string eowu::builder::fragment(const eowu::Mesh &mesh, const eowu::Material &material) {
  eowu::schema::Fragment f_schema;
  
  bool has_uv = mesh.HasAttribute(eowu::attributes::uv);
  
  f_schema.version = "330 core";
  
  if (has_uv) {
    f_schema.inputs.push_back({"tx", eowu::glsl::vec2});
  }
  
  auto material_schema = material.GetAttributeSchema();
  
  for (const auto &uniform : material_schema) {
    f_schema.uniforms.push_back(uniform);
    
    if (uniform.name == eowu::uniforms::face_color) {
      auto face_color_extraction = eowu::components::to_vec3(uniform.name, uniform.type);
      f_schema.main.push_back("FragColor = vec4(" + face_color_extraction + ", 1.0f);");
    }
  }
  
  f_schema.outputs.push_back({"FragColor", eowu::glsl::vec4});
  
  return eowu::parser::fragment(f_schema);
}

std::shared_ptr<eowu::Program> eowu::builder::from_source(const std::string &v_src, const std::string &f_src) {
  eowu::Shader v_shader;
  eowu::Shader f_shader;
  
  v_shader.Create(v_src.c_str(), eowu::shader_types::vertex);
  f_shader.Create(f_src.c_str(), eowu::shader_types::fragment);
  
  auto prog = std::make_shared<eowu::Program>();
  
  prog->Link({&v_shader, &f_shader});
  
  return prog;
}
