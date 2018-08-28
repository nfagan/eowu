//
//  Program.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Shader.hpp"
#include <eowu-common/types.hpp>
#include <vector>

namespace eowu {
  class Program;
}

class eowu::Program {
public:
  Program() = default;
  ~Program() = default;
  
  void Bind();
  void Unbind();
  void Link(const std::vector<eowu::Shader*>& shaders);
  
  void SetUniform(eowu::u32 value) const;
  
private:
  eowu::u32 id;
  
  static void check_link_errors(eowu::u32 id);
};
