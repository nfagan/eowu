//
//  Attribute.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <vector>

namespace eowu {
  class Attribute;
  
  using AttributeComponentType = eowu::f32;
  using AttributeAggregateType = std::vector<eowu::f32>;
}

class eowu::Attribute {
public:
  Attribute(const char *kind, const eowu::AttributeAggregateType &components);
  Attribute(const char *kind, const eowu::AttributeComponentType *components, eowu::u64 n_components);
  
  ~Attribute() = default;
  
  bool Matches(const eowu::Attribute& other) const;
  const char* GetKind() const;
  const eowu::AttributeAggregateType& GetComponents() const;
  
  eowu::u64 Size() const;
  
private:
  const char* kind;
  eowu::AttributeAggregateType components;
};
