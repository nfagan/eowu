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
  namespace fs {
    std::string get_eowu_root_directory(bool *success);
    std::string get_eowu_root_directory();
    
    std::string get_outer_directory(const std::string &inner_dir, bool *success);
    std::string get_outer_directory(const std::string &inner_dir, std::size_t n_levels, bool *success);
    std::string get_outer_directory(const std::string &inner_dir);
    
    bool require_directory(const std::string &path);
    
    bool directory_exists(const std::string &path);
    std::string nonexistent_directory_message(const std::string &path, const std::string &type);
    
    std::string full_file(const std::vector<std::string> &components);
    
    namespace priv {
      bool require_directory_windows(const std::string &path);
      bool require_directory_unix(const std::string &path);
    }
  }
}
