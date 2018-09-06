//
//  Util.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>

namespace eowu {
  
  struct ValidationResult {
    bool success;
    std::string message;
    std::string context;
    
    ValidationResult() : success(false) {}
  };
  
  namespace validate {
    std::string get_message_unrecognized_one_of(const std::string &kind, const std::string &value);
    std::string get_message_not_at_least_size(const std::string &kind, std::size_t expected, std::size_t actual);
    
    template<typename T>
    std::vector<std::string> get_keys(const std::unordered_map<std::string, T> &kv);
    
    ValidationResult check_unrecognized_one_of(const std::vector<std::string> &value_set,
                                               const std::string &value,
                                               const std::string &kind);
    
    template<typename T>
    ValidationResult check_at_least_size(const T &val, std::size_t expected, const std::string &kind);
  }
}

template<typename T>
eowu::ValidationResult eowu::validate::check_at_least_size(const T &val, std::size_t expected, const std::string &kind) {
  eowu::ValidationResult result;
  
  std::size_t actual = val.size();
  
  if (actual < expected) {
    result.message = eowu::validate::get_message_not_at_least_size(kind, expected, actual);
    return result;
  }
  
  result.success = true;
  
  return result;
}

template<typename T>
std::vector<std::string> eowu::validate::get_keys(const std::unordered_map<std::string, T> &kv) {
  std::vector<std::string> res;
  
  for (const auto &it : kv) {
    res.push_back(it.first);
  }
  
  return res;
}
