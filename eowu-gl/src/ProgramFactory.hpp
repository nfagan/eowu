//
//  ProgramFactory.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Program.hpp"
#include "ShaderLibrary.hpp"
#include "ShaderTypes.hpp"

namespace eowu {
  namespace program_factory {
    static eowu::Program make_debug() {
      auto frag_src = eowu::shaders::debug::fragment;
      auto vert_src = eowu::shaders::debug::vertex;
      
      eowu::Shader frag;
      eowu::Shader vert;
      eowu::Program prog;
      
      frag.Create(frag_src, eowu::shader_types::fragment);
      vert.Create(vert_src, eowu::shader_types::vertex);
      
      prog.Link({&frag, &vert});
      
      return prog;
    }
  }
}
