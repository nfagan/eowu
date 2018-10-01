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
  opacity.SetContents(eowu::uniforms::opacity, 1.0f);
}

eowu::Material::Material(const eowu::Material &other) :
face_color(other.face_color), opacity(other.opacity) {
  //
}

eowu::Material::Material(const std::shared_ptr<eowu::Material> &other) :
face_color(other->face_color), opacity(other->opacity) {
  //
}

void eowu::Material::SetOpacity(float value) {
  opacity.SetValue(value);
  opacity.SetEnabled(true);
}

void eowu::Material::Configure(eowu::Program &prog) {
  if (face_color.IsEnabled()) {
    auto fv = face_color.GetValue();
    auto fc = eowu::variant_cast(fv);
    
    unsigned int n_textures = 0;
    
    bool is_tex = eowu::uniform_is_texture(fv);
    
    if (is_tex) {
      auto& val = mpark::get<eowu::Texture>(fv);
      val.SetIndex(n_textures++);
      val.Bind();
    }
    
    prog.SetUniform(face_color.GetName(), fc);
  }
  
  if (opacity.IsEnabled()) {
    prog.SetUniform(opacity.GetName(), eowu::variant_cast(opacity.GetValue()));
  }
}

std::vector<eowu::schema::GLSLIdentifier> eowu::Material::GetAttributeSchema() const {
  std::vector<eowu::schema::GLSLIdentifier> schemas;
  
  if (face_color.IsEnabled()) {
    schemas.push_back(face_color.GetAttributeSchema());
  }
  
  if (opacity.IsEnabled()) {
    schemas.push_back(opacity.GetAttributeSchema());
  }
  
  return schemas;
}

bool eowu::Material::SchemaChanged() const {
  return face_color.StateChanged() || opacity.StateChanged();
}

void eowu::Material::NextFrame() {
  face_color.NextFrame();
  opacity.NextFrame();
}

const eowu::Identifier& eowu::Material::GetIdentifier() const {
  return resource_id;
}

void eowu::Material::MakeLike(const eowu::Material &other) {
  face_color = other.face_color;
  opacity = other.opacity;
}
