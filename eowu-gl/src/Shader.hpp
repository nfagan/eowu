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
  
  void Create(const char *source, eowu::u32);
  void Dispose();
  void Attach(eowu::u32 program_id) const;
  
  bool IsCreated() const;
  
private:
  bool is_created;
  eowu::u32 id;
  
  static eowu::u32 get_gl_id_from_type(eowu::u32 type);
  static void validate_shader(eowu::u32 id);
};
