//
//  SetupStatus.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <string>

namespace eowu {
  struct SetupStatus;
}

struct eowu::SetupStatus {
  SetupStatus();
  SetupStatus(const std::string &file,
              const std::string &msg,
              const std::string &context);
  
  bool success;
  std::string file;
  std::string message;
  std::string context;
  
  void print() const;
  
  static SetupStatus entry_file_error(const std::string &msg, const std::string &file);
  static SetupStatus entry_parse_error(const std::string &msg,
                                       const std::string &file,
                                       const std::string &context);
  static SetupStatus entry_file_validation_error(const std::string &file,
                                                 const std::string &msg,
                                                 const std::string &context);
};
