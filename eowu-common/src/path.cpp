//
//  path.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "path.hpp"
#include "platform.hpp"
#include <string_view>
#include <sys/types.h>
#include <sys/stat.h>

std::string eowu::path::get_eowu_root_directory(bool *success) {
  std::string file = __FILE__;
  return eowu::path::get_outer_directory(file, 3, success);
}

std::string eowu::path::get_eowu_root_directory() {
  bool success;
  
  std::string result = get_eowu_root_directory(&success);
  
  if (!success) {
    return "";
  }
  
  return result;
}

std::string eowu::path::get_outer_directory(const std::string &inner_dir, std::size_t n_levels, bool *success) {
  if (n_levels == 0) {
    *success = true;
    return inner_dir;
  }
  
  std::string result = eowu::path::get_outer_directory(inner_dir, success);
  
  if (!success) {
    return result;
  }
  
  for (std::size_t i = 1; i < n_levels; i++) {
    result = eowu::path::get_outer_directory(result, success);
    
    if (!success) {
      return result;
    }
  }
  
  return result;
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

bool eowu::path::directory_exists(const std::string &path) {
  struct stat info;
  const char *pathname = path.c_str();
  
  if (stat(pathname, &info) != 0) {
    return false;
  } else if (info.st_mode & S_IFDIR) {
    return true;
  } else {
    return false;
  }
}

std::string eowu::path::full_file(const std::vector<std::string> &components) {
  std::string result;
  std::string slash = eowu::path::slash;
  
  const std::size_t n_components = components.size();
  
  for (std::size_t i = 0; i < n_components; i++) {
    const std::string &component = components[i];
    
    auto sz = component.length();
    
    if (sz == 0) {
      continue;
    }
    
    std::string processed = component;
    bool should_append = true;
    
    while (true) {
      std::size_t processed_sz = processed.length();
      
      if (processed_sz == 0) {
        should_append = false;
        break;
      }
      
      if (&processed[processed_sz-1] == slash) {
        processed.pop_back();
      } else {
        break;
      }
    }
    
    if (should_append) {
      if (i > 0) {
        result += slash;
      }
      result += processed;
    }
  }
  
  return result;
}
