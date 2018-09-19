//
//  fs.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "fs.hpp"
#include "platform.hpp"
#include <string_view>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef EOWU_IS_WIN
#include <windows.h>
#endif

std::string eowu::fs::get_eowu_root_directory(bool *success) {
  std::string file = __FILE__;
  return eowu::fs::get_outer_directory(file, 3, success);
}

std::string eowu::fs::get_eowu_root_directory() {
  bool success;
  
  std::string result = get_eowu_root_directory(&success);
  
  if (!success) {
    return "";
  }
  
  return result;
}

std::string eowu::fs::get_outer_directory(const std::string &inner_dir, std::size_t n_levels, bool *success) {
  if (n_levels == 0) {
    *success = true;
    return inner_dir;
  }
  
  std::string result = eowu::fs::get_outer_directory(inner_dir, success);
  
  if (!success) {
    return result;
  }
  
  for (std::size_t i = 1; i < n_levels; i++) {
    result = eowu::fs::get_outer_directory(result, success);
    
    if (!success) {
      return result;
    }
  }
  
  return result;
}

std::string eowu::fs::get_outer_directory(const std::string &inner_dir, bool *success) {
  *success = false;
  
  if (inner_dir.length() == 0) {
    return inner_dir;
  }
  
  std::string_view dir_view = inner_dir;
  std::string slash = eowu::fs::slash;
  
  while (true) {
    auto sz = dir_view.length();
    
    if (sz == 0) {
      return "";
    }
    
    const char *last_char = &dir_view[sz-1];
    
    if (slash == last_char) {
      dir_view.remove_suffix(1);
    } else {
      break;
    }
  }
  
  auto index = dir_view.find_last_of(slash);
  
  if (index >= dir_view.length()) {
    return inner_dir;
  }

  *success = true;
  
  return std::string(dir_view.substr(0, index));
}

std::string eowu::fs::get_outer_directory(const std::string &inner_dir) {
  bool dummy;
  return eowu::fs::get_outer_directory(inner_dir, &dummy);
}

bool eowu::fs::directory_exists(const std::string &path) {
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

std::string eowu::fs::nonexistent_directory_message(const std::string &path, const std::string &type) {
  return "Directory '" + path + "' of type '" + type + "' does not exist.";
}

std::string eowu::fs::full_file(const std::vector<std::string> &components) {
  std::string result;
  std::string slash = eowu::fs::slash;
  
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
      
      std::string_view last(processed);
      std::string_view first(processed);
      
      last = last.substr(processed_sz-1, 1);
      first = first.substr(0, 1);
      
      if (last == slash) {
        processed.pop_back();
      } else if (first == slash && i > 0) {
        processed.erase(0, 1);
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

bool eowu::fs::require_directory(const std::string &path) {
#ifdef EOWU_IS_WIN
  return eowu::fs::priv::require_directory_windows(path);
#else
  return eowu::fs::priv::require_directory_unix(path);
#endif
}

#ifdef EOWU_IS_WIN
bool eowu::fs::priv::require_directory_windows(const std::string &path) {
  const char *path_str = path.c_str();
  
  if (CreateDirectory(path_str, NULL)) {
    return true;
  } else {
    return false;
  }
}
#else
bool eowu::fs::priv::require_directory_unix(const std::string &path) {
  // https://codeyarns.com/2014/08/07/how-to-create-directory-using-c-on-linux/
  const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  
  if (-1 == dir_err) {
    return false;
  }
  
  return true;
}
#endif
