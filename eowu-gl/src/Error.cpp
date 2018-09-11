//
//  Error.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/11/18.
//

#include "Error.hpp"

eowu::NonexistentResourceError eowu::NonexistentResourceError::MessageKindId(const std::string &kind, const std::string &id) {
  return eowu::NonexistentResourceError(kind + ": '" + id + "' does not exist.");
}
