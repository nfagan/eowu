//
//  Attribute.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Attribute.hpp"

eowu::Attribute::Attribute(const char *kind, const std::vector<eowu::f32> &components) {
  this->kind = kind;
  this->components = components;
}

bool eowu::Attribute::Matches(const eowu::Attribute& other) const {
  
  if (other.Size() != Size()) {
    return false;
  }
  
  if (strcmp(other.GetKind(), GetKind()) != 0) {
    return false;
  }
  
  return true;
}

const char* eowu::Attribute::GetKind() const {
  return kind;
}

const std::vector<eowu::f32>& eowu::Attribute::GetComponents() const {
  return components;
}

eowu::u64 eowu::Attribute::Size() const {
  return components.size();
}
