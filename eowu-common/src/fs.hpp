//
//  fs.hpp
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
    
    struct finfo {
      std::string name;
      std::string directory;
      bool is_directory;
      
      finfo();
      ~finfo() = default;
    };
    
    std::vector<eowu::fs::finfo> get_directory_contents(const std::string &path, bool is_recursive, bool *success);
    std::vector<eowu::fs::finfo> get_directory_contents(const std::string &path, bool is_recursive = false);
    
    namespace priv {
      void get_directory_contents_impl(const std::string &path,
                                       std::vector<eowu::fs::finfo> &contents,
                                       bool is_recursive,
                                       bool *success);
    }
  }
}
