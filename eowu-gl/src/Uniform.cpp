//
//  Uniform.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Uniform.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

eowu::Uniform::Uniform() {
  has_value = false;
}

eowu::Uniform::Uniform(const std::string &name, const UniformVariantType &value) {
  SetContents(name, value);
}

void eowu::Uniform::SetName(const std::string &name) {
  this->name = name;
}

void eowu::Uniform::SetValue(const UniformVariantType &value) {
  this->value = value;
  has_value = true;
}

void eowu::Uniform::SetContents(const std::string &name, const UniformVariantType &value) {
  SetName(name);
  SetValue(value);
}

const std::string& eowu::Uniform::GetName() const {
  return name;
}

void eowu::Uniform::Set(eowu::u32 loc) const {
  
  auto index = value.index();
  
  if (index == 0) {
    glUniform1i(loc, mpark::get<0>(value));
  } else if (index == 1) {
    auto val = mpark::get<1>(value);
    glUniform3f(loc, val.x, val.y, val.z);
  } else if (index == 2) {
    auto val = mpark::get<2>(value);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  } else if (index == 3) {
    glUniform1i(loc, mpark::get<3>(value));
  } else if (index == 4) {
    glUniform1f(loc, mpark::get<4>(value));
  }
  
}
