//
//  Fonts.hpp
//  eowu
//
//  Created by Nick Fagan on 12/23/18.
//

#pragma once

#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct Font {
      std::string file_path;
      
      Font(const std::string &path) : file_path(path) {}
    };
    
    struct Fonts {
      std::unordered_map<std::string, eowu::schema::Font> fonts;
    };
  }
}
