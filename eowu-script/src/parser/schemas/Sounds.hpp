//
//  Sounds.hpp
//  eowu
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct Sounds {
      std::unordered_map<std::string, std::string> mapping;
    };
  }
}
