//
//  SetupValidator.hh
//  eowu
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

template<typename T>
eowu::ValidationResult eowu::validate::map_check_duplicates(std::unordered_set<std::string> &visited,
                                                            const std::unordered_map<std::string, T> &to_check,
                                                            const std::string &kind,
                                                            const std::string &context) {
  eowu::ValidationResult result;
  
  for (const auto &it : to_check) {
    const auto &id = it.first;
    
    if (visited.count(id) > 0) {
      result.message = "Duplicate " + kind + ": '" + id + "'.";
      result.context = context;
      return result;
    }
    
    visited.emplace(id);
  }
  
  result.success = true;
  
  return result;
}
