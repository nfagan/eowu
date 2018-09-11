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
  
  class NonexistentResourceError : public std::runtime_error {
  public:
    NonexistentResourceError(const std::string &msg) : std::runtime_error(msg) {}
    static NonexistentResourceError MessageKindId(const std::string &kind, const std::string &id);
  };
  
  EOWU_GENERIC_ERROR(InteralContextError);
  EOWU_GENERIC_ERROR(UnrecognizedLabelError);
}
