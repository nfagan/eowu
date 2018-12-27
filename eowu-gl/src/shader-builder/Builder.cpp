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
  
  //  inputs
  if (has_uv) {
    f_schema.inputs.push_back({"tx", eowu::glsl::vec2});
  }
  
  //  uniforms + opacity in main
  configure_fragment_material(f_schema, material);
  
  //  outputs
  f_schema.outputs.push_back({"FragColor", eowu::glsl::vec4});
  
  return eowu::parser::fragment(f_schema);
}

std::string eowu::builder::vertex_text(const eowu::Material &material) {
  eowu::schema::Vertex v_schema;
  
  v_schema.version = "330 core";
  
  //  attributes
  v_schema.attributes.push_back({"0", eowu::attributes::position, eowu::glsl::vec4});
  
  //  uniforms
  v_schema.uniforms.push_back({eowu::uniforms::projection, eowu::glsl::mat4});
  
  //  outputs
  v_schema.outputs.push_back({"tx", eowu::glsl::vec2});
  
  //  main
  v_schema.main.push_back("tx = position.zw;");
  v_schema.main.push_back("gl_Position = projection * vec4(position.xy, 0.0f, 1.0f);");
  
  return eowu::parser::vertex(v_schema);
}

std::string eowu::builder::fragment_text(const eowu::Material &material) {
  eowu::schema::Fragment f_schema;
  
  f_schema.version = "330 core";
  
  //  inputs
  f_schema.inputs.push_back({"tx", eowu::glsl::vec2});
  
  //  uniforms
  auto material_schema = material.GetAttributeSchema();
  
  for (const auto &uniform : material_schema) {
    f_schema.uniforms.push_back(uniform);
  }
  
  //  text-specific
  f_schema.uniforms.push_back({eowu::uniforms::text, eowu::glsl::sampler2D});
  
  std::string sample_texture = "texture(" + std::string(eowu::uniforms::text) + ", tx).r";
  f_schema.main.push_back("vec4 sampled_alpha = vec4(1.0f, 1.0f, 1.0f, " + sample_texture + ");");
  f_schema.main.push_back("FragColor = vec4(" + std::string(eowu::uniforms::face_color) + ", 1.0f) * sampled_alpha;");
  
  //  outputs
  f_schema.outputs.push_back({"FragColor", eowu::glsl::vec4});
  
  return eowu::parser::fragment(f_schema);
}

void eowu::builder::configure_fragment_material(eowu::schema::Fragment &f_schema, const eowu::Material &material) {
  auto material_schema = material.GetAttributeSchema();
  
  //  uniforms
  for (const auto &uniform : material_schema) {
    f_schema.uniforms.push_back(uniform);
    
    if (uniform.name == eowu::uniforms::face_color) {
      auto face_color_extraction = eowu::components::to_vec3(uniform.name, uniform.type);
      f_schema.main.push_back("FragColor = vec4(" + face_color_extraction + ", opacity);");
    }
  }
  
  //  define opacity if present
  if (!eowu::builder::has_identifier(material_schema, "opacity")) {
    f_schema.main.push_back(eowu::components::declare_variable("opacity", eowu::glsl::float_t, "1.0f"));
  }
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

bool eowu::builder::has_identifier(const std::vector<eowu::schema::GLSLIdentifier> &identifiers, const char *name) {
  for (const auto &id : identifiers) {
    if (id.name == name) {
      return true;
    }
  }
  
  return false;
}
