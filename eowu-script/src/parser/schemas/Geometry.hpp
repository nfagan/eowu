//
//  Geometry.hpp
//  eowu
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct BuiltinGeometry {
      std::unordered_map<std::string, std::string> mapping;
    };
    
    struct Geometry {
      BuiltinGeometry builtins;
    };
  }
}
