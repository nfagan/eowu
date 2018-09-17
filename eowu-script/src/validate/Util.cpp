//
//  Util.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Util.hpp"
#include <algorithm>

std::string eowu::validate::get_message_unrecognized_one_of(const std::string &kind, const std::string &value) {
  return "'" + value + "' is not a recognized '" + kind + "'.";
}

std::string eowu::validate::get_message_not_at_least_size(const std::string &kind, std::size_t expected, std::size_t actual) {
  return "Expected '" + kind + "' to have at least " + std::to_string(expected) + " elements; " + std::to_string(actual) + " were present.";
}

eowu::ValidationResult eowu::validate::check_unrecognized_one_of(const std::unordered_set<std::string> &value_set,
                                                                 const std::string &value,
                                                                 const std::string &kind) {
  eowu::ValidationResult result;
  
  if (value_set.count(value) == 0) {
    result.message = validate::get_message_unrecognized_one_of(kind, value);
    return result;
  }
  
  result.success = true;
  
  return result;
}

eowu::ValidationResult eowu::validate::check_unrecognized_one_of(const std::vector<std::string> &value_set,
                                                                 const std::string &value,
                                                                 const std::string &kind) {
  eowu::ValidationResult result;
  
  const auto beg = value_set.begin();
  const auto end = value_set.end();
  
  bool some_of = std::any_of(beg, end, [&](auto &v) {
    return v == value;
  });
  
  if (!some_of) {
    result.message = validate::get_message_unrecognized_one_of(kind, value);
    return result;
  }
  
  result.success = true;
  
  return result;
}
