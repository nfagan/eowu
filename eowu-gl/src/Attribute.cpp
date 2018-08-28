//
//  Attribute.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Attribute.hpp"

eowu::Attribute::Attribute(const char *kind, const eowu::AttributeAggregateType &components) {
  this->kind = kind;
  this->components = components;
}

eowu::Attribute::Attribute(const char *kind, const eowu::AttributeComponentType *components, eowu::u64 n_components) {
  eowu::AttributeAggregateType vec_components;
  
  for (eowu::u64 i = 0; i < n_components; i++) {
    vec_components.push_back(components[i]);
  }
  
  this->kind = kind;
  this->components = std::move(vec_components);
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

const eowu::AttributeAggregateType& eowu::Attribute::GetComponents() const {
  return components;
}

eowu::u64 eowu::Attribute::Size() const {
  return components.size();
}
