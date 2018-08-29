//
//  Program.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Shader.hpp"
#include "Uniform.hpp"
#include <eowu-common/types.hpp>
#include <vector>
#include <unordered_map>
#include <string>

namespace eowu {
  class Program;
}

class eowu::Program {
public:
  Program();
  ~Program();
  
  void Dispose();
  void Bind();
  void Unbind();
  void Link(const std::vector<eowu::Shader*>& shaders);
  bool IsFinalized() const;
  
  bool HasUniform(const std::string &name);
  void SetUniform(const eowu::Uniform &uniform);
  void SetUniform(const std::string &name, const UniformVariantType &value);
  
private:
  eowu::u32 id;
  std::unordered_map<std::string, eowu::s32> uniform_locations;
  
  bool is_finalized;
  
  static void check_link_errors(eowu::u32 id);
};
