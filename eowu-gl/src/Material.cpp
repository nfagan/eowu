//
//  Material.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/30/18.
//

#include "Material.hpp"
#include "Uniform.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "UniformNames.hpp"
#include "Variant.hpp"

eowu::Material::Material() {
  face_color.SetContents(eowu::uniforms::face_color, glm::vec3(0.0f));
}

eowu::Material::Material(const eowu::Material &other) : face_color(other.face_color) {};
eowu::Material::Material(const std::shared_ptr<eowu::Material> &other) : face_color(other->face_color) {};

void eowu::Material::Configure(eowu::Program &prog) {
  auto fc = eowu::variant_cast(face_color.GetValue());
  prog.SetUniform(face_color.GetName(), fc);
}

std::vector<eowu::schema::GLSLIdentifier> eowu::Material::GetAttributeSchema() const {
  auto face_color_schema = face_color.GetAttributeSchema();
  
  return {face_color_schema};
}

bool eowu::Material::SchemaChanged() const {
  return face_color.TypeChanged();
}

void eowu::Material::NextFrame() {
  face_color.NextFrame();
}

const eowu::Identifier& eowu::Material::GetIdentifier() const {
  return resource_id;
}
