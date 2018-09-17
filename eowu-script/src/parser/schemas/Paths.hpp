//
//  Paths.hpp
//  eowu
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace schema {
    struct Paths {
      bool provided_data;
      
      std::string data;
      
      Paths() : provided_data(false) {}
    };
  }
}
