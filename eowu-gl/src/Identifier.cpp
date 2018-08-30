//
//  Identifier.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/30/18.
//

#include "Identifier.hpp"

std::ostream& eowu::operator<<(std::ostream& os, const eowu::Identifier &id) {
  os << id.numeric_id;
  return os;
}
