//
//  path.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#pragma once

#include <string>
#include <cstddef>
#include <vector>

namespace eowu {
  namespace path {
    std::string get_eowu_root_directory(bool *success);
    std::string get_eowu_root_directory();
    
    std::string get_outer_directory(const std::string &inner_dir, bool *success);
    std::string get_outer_directory(const std::string &inner_dir, std::size_t n_levels, bool *success);
    std::string get_outer_directory(const std::string &inner_dir);
    
    bool directory_exists(const std::string &path);
    
    std::string full_file(const std::vector<std::string> &components);
  }
}
