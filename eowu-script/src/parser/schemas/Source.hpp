//
//  Source.hpp
//  eowu
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct Source {
      std::string type;
      std::string source_id;
      std::string window_id;
      
      bool provided_window_id;
      
      Source() : provided_window_id(false) {}
    };
    
    struct Sources {
      std::unordered_map<std::string, eowu::schema::Source> sources;
    };
  }
}
