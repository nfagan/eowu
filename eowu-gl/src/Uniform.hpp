//
//  Uniform.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <mpark/variant.hpp>
#include <glm/glm.hpp>
#include <string>

namespace eowu {
  class Uniform;
  
  using UniformVariantType = mpark::variant<bool, glm::vec3, glm::mat4, eowu::s32, eowu::f32>;
}

class eowu::Uniform {
public:
  Uniform();
  Uniform(const std::string &name, const eowu::UniformVariantType &value);
  
  ~Uniform() = default;
  
  void SetName(const std::string &name);
  void SetValue(const eowu::UniformVariantType &value);
  void SetContents(const std::string &name, const eowu::UniformVariantType &value);
  const std::string& GetName() const;
  
  void Set(eowu::u32 loc) const;
private:
  std::string name;
  eowu::UniformVariantType value;
  bool has_value;
};
