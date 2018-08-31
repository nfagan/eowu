//
//  Uniform.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Uniform.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

struct UniformSettingImplementation {
  unsigned int loc;
  
  UniformSettingImplementation(unsigned int loc_) : loc(loc_) {};
  
  void operator()(bool val) {
    glUniform1i(loc, val);
  }
  void operator()(int val) {
    glUniform1i(loc, val);
  }
  void operator()(float val) {
    glUniform1f(loc, val);
  }
  void operator()(const glm::vec3 &val) {
    glUniform3f(loc, val.x, val.y, val.z);
  }
  void operator()(const glm::mat4 &val) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
  }
  void operator()(const eowu::Texture &tex) {
    glUniform1i(loc, tex.GetIndex());
  }
};

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

void eowu::Uniform::Set(unsigned int loc) const {
  mpark::visit(UniformSettingImplementation{loc}, value);
}
