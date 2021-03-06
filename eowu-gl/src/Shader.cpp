//
//  Shader.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Shader.hpp"
#include "Error.hpp"
#include <glad/glad.h>

eowu::Shader::Shader() {
  is_created = false;
}

eowu::Shader::~Shader() {
//  Dispose();
}

bool eowu::Shader::IsCreated() const {
  return is_created;
}

void eowu::Shader::Attach(unsigned int program_id) const {
  if (IsCreated()) {
    glAttachShader(program_id, id);
  } else {
    throw eowu::ShaderNotCompiledError("Shader not yet compiled.");
  }
}

void eowu::Shader::Dispose() {
  if (IsCreated()) {
    glDeleteShader(id);
    is_created = false;
  }
}

void eowu::Shader::Create(const char *source, unsigned int type) {
  if (IsCreated()) {
    return;
  }
  
  id = glCreateShader(get_gl_id_from_type(type));
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);
  
  validate_shader(id);
  
  is_created = true;
}
                      
unsigned int eowu::Shader::get_gl_id_from_type(unsigned int type) {
  if (type == eowu::shader_types::vertex) {
    return GL_VERTEX_SHADER;
  } else if (type == eowu::shader_types::fragment) {
    return GL_FRAGMENT_SHADER;
  } else {
    return 0;
  }
}

void eowu::Shader::validate_shader(unsigned int id) {
  int success;
  char info_log[1024];
  
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  
  if (!success) {
    glGetShaderInfoLog(id, 1024, NULL, info_log);
    
    throw eowu::ShaderCompilationError(info_log);
  }
}
