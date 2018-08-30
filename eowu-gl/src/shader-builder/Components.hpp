//
//  Components.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Types.hpp"

namespace eowu {
  namespace components {
    std::string declare_version(const std::string &version);
    std::string declare_attribute(const std::string &loc, const std::string &name, eowu::glsl::Types type);
    std::string declare_uniform(const std::string &name, eowu::glsl::Types type);
    std::string declare_variable(const std::string &name, eowu::glsl::Types type);
    std::string to_vec3(const std::string &name, eowu::glsl::Types type);
    
    std::string begin_main();
    std::string open_brace();
    std::string close_brace();
  }
}
