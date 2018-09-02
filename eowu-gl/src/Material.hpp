//
//  Material.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "MaterialAttribute.hpp"
#include "Texture.hpp"
#include "Identifier.hpp"
#include "Constraints.hpp"
#include <glm/glm.hpp>

namespace eowu {
  class Material;
  class Program;
  
  namespace schema {
    struct GLSLIdentifier;
  }
}

class eowu::Material {
public:
  Material();
  Material(const eowu::Material &other);
  Material(const std::shared_ptr<eowu::Material> &ptr);
  ~Material() = default;
  
  void Configure(eowu::Program &prog);
  std::vector<eowu::schema::GLSLIdentifier> GetAttributeSchema() const;
  
  bool SchemaChanged() const;
  void NextFrame();
  
  const eowu::Identifier& GetIdentifier() const;
  
  template<typename T>
  void SetFaceColor(const T &value);
  
private:
  eowu::MaterialAttribute<glm::vec3, eowu::Texture> face_color;
  eowu::Identifier resource_id;
};

template<typename T>
void eowu::Material::SetFaceColor(const T &value) {  
  face_color.SetValue(value);
}

