//
//  path.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "path.hpp"
#include "platform.hpp"
#include <string_view>

std::string eowu::path::get_eowu_root_directory() {
  std::string file = __FILE__;
  
  
}

std::string eowu::path::get_outer_directory(const std::string &inner_dir, bool *success) {
  *success = false;
  
  const auto sz = inner_dir.length();
  
  if (sz == 0) {
    return inner_dir;
  }
  
  std::string_view dir_view = inner_dir;
  std::string slash = eowu::path::slash;
  
  const char *last_char = &dir_view[sz-1];
  
  if (slash == last_char) {
    dir_view.remove_suffix(1);
  }
  
  auto index = dir_view.find_last_of(slash);
  
  if (index >= sz) {
    return inner_dir;
  }

  *success = true;
  
  return std::string(dir_view.substr(0, index));
}

std::string eowu::path::get_outer_directory(const std::string &inner_dir) {
  bool dummy;
  return eowu::path::get_outer_directory(inner_dir, &dummy);
}
