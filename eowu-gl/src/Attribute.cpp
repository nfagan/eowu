//
//  Attribute.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Attribute.hpp"

eowu::Attribute::Attribute(const std::string &kind, const eowu::AttributeAggregateType &components) {
  this->kind = kind;
  this->components = components;
}

eowu::Attribute::Attribute(const std::string &kind, const eowu::AttributeComponentType *components, std::size_t n_components) {
  eowu::AttributeAggregateType vec_components;
  
  for (std::size_t i = 0; i < n_components; i++) {
    vec_components.push_back(components[i]);
  }
  
  this->kind = kind;
  this->components = std::move(vec_components);
}

bool eowu::Attribute::Matches(const eowu::Attribute& other) const {
  
  if (other.Size() != Size()) {
    return false;
  }
  
  return other.GetKind() == GetKind();
}

const std::string& eowu::Attribute::GetKind() const {
  return kind;
}

const eowu::AttributeAggregateType& eowu::Attribute::GetComponents() const {
  return components;
}

std::size_t eowu::Attribute::Size() const {
  return components.size();
}
