//
//  path.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace path {
    std::string get_eowu_root_directory();
    
    std::string get_outer_directory(const std::string &inner_dir, bool *success);
    std::string get_outer_directory(const std::string &inner_dir);
  }
}
