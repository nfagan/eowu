//
//  Program.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Program.hpp"
#include "Error.hpp"
#include <eowu-common/logging.hpp>
#include <glad/glad.h>
#include <utility>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

eowu::Program::Program() {
  is_finalized = false;
}

eowu::Program::~Program() {
  Dispose();
}

void eowu::Program::Dispose() {
  if (IsFinalized()) {
    glDeleteProgram(id);
  }
  
  uniform_locations.clear();
  is_finalized = false;
}

void eowu::Program::Link(const std::vector<eowu::Shader*>& shaders) {
  if (IsFinalized()) {
    Dispose();
  }
  
  id = glCreateProgram();
  
  for (const auto& shader : shaders) {
    shader->Attach(id);
  }
  
  glLinkProgram(id);
  
  for (const auto& shader : shaders) {
    shader->Dispose();
  }
  
  is_finalized = true;
}

const eowu::Identifier& eowu::Program::GetIdentifier() const {
  return resource_id;
}

bool eowu::Program::IsFinalized() const {
  return is_finalized;
}

bool eowu::Program::HasUniform(const std::string &name) {
  if (!IsFinalized()) {
    return false;
  }
  
  const auto& it = uniform_locations.find(name);
  
  if (it == uniform_locations.end()) {
    int loc = glGetUniformLocation(id, name.c_str());
    
    if (loc == -1) {
      return false;
    }
    
    uniform_locations[name] = loc;
  }
  
  return true;
}

void eowu::Program::SetUniform(const std::string &name, const UniformVariantType &value) {
  SetUniform(eowu::Uniform(name, value));
}

void eowu::Program::SetUniform(const eowu::Uniform &uniform) {
  const auto& name = uniform.GetName();
  
  if (!HasUniform(name)) {
    EOWU_LOG_WARN("Program::SetUniform: Missing uniform: " << name);
    return;
  }
  
  int loc = uniform_locations.at(name);
  uniform.Set(loc);
}

void eowu::Program::Bind() {
  if (!IsFinalized()) {
    throw new ProgramNotFinalizedError("Attempted to bind an invalid / non-finalized program.");
  }
  
  glUseProgram(id);
}

void eowu::Program::Unbind() {
  if (!IsFinalized()) {
    throw new ProgramNotFinalizedError("Attempted to un-bind an invalid / non-finalized program.");
  }
  
  glUseProgram(0);
}

void eowu::Program::check_link_errors(unsigned int id) {
  int success;
  char info_log[1024];
  
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, info_log);
    
    throw eowu::ProgramLinkError(info_log);
  }
}
