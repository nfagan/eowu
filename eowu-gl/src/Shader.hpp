//
//  Shader.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "ShaderTypes.hpp"
#include <eowu-common/types.hpp>

namespace eowu {
  class Shader;
}

class eowu::Shader {
public:
  Shader();
  ~Shader();
  
  void Create(const char *source, unsigned int type);
  void Dispose();
  void Attach(unsigned int program_id) const;
  
  bool IsCreated() const;
  
private:
  bool is_created;
  unsigned int id;
  
  static unsigned int get_gl_id_from_type(unsigned int type);
  static void validate_shader(unsigned int id);
};
