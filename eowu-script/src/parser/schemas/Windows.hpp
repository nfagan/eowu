//
//  Windows.hpp
//  eowu
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct Window {
      unsigned int index;
      unsigned int full_screen;
      int width;
      int height;
    };
    
    struct Windows {
      std::unordered_map<std::string, eowu::schema::Window> windows;
    };
  }
}
