//
//  Attribute.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Attribute.hpp"

eowu::Attribute::Attribute(const std::string &kind_, const eowu::AttributeAggregateType &components_) :
kind(kind_),
components(components_) {
  //
}

eowu::Attribute::Attribute(const std::string &kind_,
                           const eowu::AttributeComponentType *components,
                           std::size_t n_components) :
kind(kind_) {
  
  eowu::AttributeAggregateType vec_components;
  
  for (std::size_t i = 0; i < n_components; i++) {
    vec_components.push_back(components[i]);
  }
  
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
