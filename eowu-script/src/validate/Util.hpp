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
#include <unordered_set>
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
    std::unordered_set<std::string> get_keys(const std::unordered_map<std::string, T> &kv);
    
    template<typename K, typename V>
    std::vector<V> get_values(const std::unordered_map<K, V> &kv);
    
    ValidationResult check_unrecognized_one_of(const std::unordered_set<std::string> &value_set,
                                               const std::string &value,
                                               const std::string &kind);
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
std::unordered_set<std::string> eowu::validate::get_keys(const std::unordered_map<std::string, T> &kv) {
  std::unordered_set<std::string> res;
  
  for (const auto &it : kv) {
    res.emplace(it.first);
  }
  
  return res;
}

template<typename K, typename V>
std::vector<V> eowu::validate::get_values(const std::unordered_map<K, V> &kv) {
  std::vector<V> res;
  
  for (const auto &it : kv) {
    res.push_back(it.second);
  }
  
  return res;
}
