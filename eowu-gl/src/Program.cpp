//
//  Program.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Program.hpp"
#include "Error.hpp"
#include <glad/glad.h>

void eowu::Program::Link(const std::vector<eowu::Shader*>& shaders) {
  id = glCreateProgram();
  
  for (const auto& shader : shaders) {
    shader->Attach(id);
  }
  
  glLinkProgram(id);
  
  for (const auto& shader : shaders) {
    shader->Dispose();
  }
}

void eowu::Program::Bind() {
  glUseProgram(id);
}

void eowu::Program::Unbind() {
  glUseProgram(0);
}

void eowu::Program::check_link_errors(eowu::u32 id) {
  int success;
  char info_log[1024];
  
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  
  if (!success) {
    glGetProgramInfoLog(id, 1024, NULL, info_log);
    
    throw eowu::ProgramLinkError(info_log);
  }
}
