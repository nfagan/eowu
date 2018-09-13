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
      bool is_fullscreen;
      bool is_resizeable;
      bool is_vsynced;
      
      int width;
      int height;
      std::string title;
    };
    
    struct Windows {
      std::unordered_map<std::string, eowu::schema::Window> windows;
    };
  }
}
