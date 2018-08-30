//
//  Attribute.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <vector>
#include <string>

namespace eowu {
  class Attribute;
  
  using AttributeComponentType = eowu::f32;
  using AttributeAggregateType = std::vector<eowu::f32>;
}

class eowu::Attribute {
public:
  Attribute(const std::string &kind, const eowu::AttributeAggregateType &components);
  Attribute(const std::string &kind, const eowu::AttributeComponentType *components, eowu::u64 n_components);
  
  ~Attribute() = default;
  
  bool Matches(const eowu::Attribute& other) const;
  const std::string& GetKind() const;
  const eowu::AttributeAggregateType& GetComponents() const;
  
  eowu::u64 Size() const;
  
private:
  std::string kind;
  eowu::AttributeAggregateType components;
};
