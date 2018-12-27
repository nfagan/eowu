//
//  Error.hpp
//  eowu
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include <stdexcept>
#include <string>
#include <eowu-common/error.hpp>

namespace eowu {
  EOWU_GENERIC_ERROR(NonMatchingVerticesError);
  EOWU_GENERIC_ERROR(ShaderCompilationError);
  EOWU_GENERIC_ERROR(ShaderNotCompiledError);
  EOWU_GENERIC_ERROR(ProgramLinkError);
  EOWU_GENERIC_ERROR(ProgramNotFinalizedError);
  
  EOWU_GENERIC_ERROR(NotImplementedError);
  
  EOWU_GENERIC_ERROR(ContextFailedToInitializeError);
  EOWU_GENERIC_ERROR(ContextNotInitializedError);
  EOWU_GENERIC_ERROR(MonitorNotAvailableError);
  EOWU_GENERIC_ERROR(WindowError);
  
  EOWU_GENERIC_ERROR(TextureLoadError);
  EOWU_GENERIC_ERROR(IncompleteModelError);
  
  class FontError : public std::runtime_error {
  public:
    FontError(const std::string &msg) : std::runtime_error(msg), message(msg) {
      //
    }
    
    FontError(const std::string &msg, int err_code) : std::runtime_error(msg), error_code(err_code) {
      make_message();
    }
    
    virtual const char* what() const noexcept override {
      return message.c_str();
    }
  private:
    int error_code;
    std::string message;
    
    void make_message() {
      std::string base_message = std::runtime_error::what();
      std::string err_code = " : Error code: " + std::to_string(error_code);
      message = base_message + err_code;
    }
  };
  
  class NonexistentResourceError : public std::runtime_error {
  public:
    NonexistentResourceError(const std::string &msg) : std::runtime_error(msg) {}
    static NonexistentResourceError MessageKindId(const std::string &kind, const std::string &id);
  };
  
  EOWU_GENERIC_ERROR(InteralContextError);
  EOWU_GENERIC_ERROR(UnrecognizedLabelError);
}
