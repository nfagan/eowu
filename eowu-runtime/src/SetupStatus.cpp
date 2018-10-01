//
//  SetupStatus.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "SetupStatus.hpp"
#include <iostream>
#include <eowu-common/debug.hpp>

eowu::SetupStatus::SetupStatus() : success(false) {}

eowu::SetupStatus::SetupStatus(const std::string &file_,
                               const std::string &msg_,
                               const std::string &ctx_) :
success(false), file(file_), message(msg_), context(ctx_) {
  //
}

void eowu::SetupStatus::print() const {
  if (success) {
    return;
  }
  
  std::cout << std::endl;
  eowu::debug::print_error("");
  
  std::cout << "Failed to initialize script: '" << file << "'" << std::endl << std::endl;
  std::cout << message << std::endl << std::endl;
  
  if (context != "") {
    std::cout << "Context: " << context << std::endl << std::endl;
  }
}

eowu::SetupStatus eowu::SetupStatus::entry_file_error(const std::string &file,
                                                      const std::string &msg) {
  return eowu::SetupStatus(file, msg, "");
}

eowu::SetupStatus eowu::SetupStatus::entry_file_validation_error(const std::string &file,
                                                                 const std::string &msg,
                                                                 const std::string &context) {
  return eowu::SetupStatus(file, msg, context);
}

eowu::SetupStatus eowu::SetupStatus::entry_parse_error(const std::string &file,
                                                       const std::string &msg,
                                                       const std::string &context) {
  return eowu::SetupStatus(file, msg, context);
}
