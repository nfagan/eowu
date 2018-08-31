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
#include <cstddef>

namespace eowu {
  class Attribute;
  
  using AttributeComponentType = float;
  using AttributeAggregateType = std::vector<float>;
}

class eowu::Attribute {
public:
  Attribute(const std::string &kind, const eowu::AttributeAggregateType &components);
  Attribute(const std::string &kind, const eowu::AttributeComponentType *components, std::size_t n_components);
  
  ~Attribute() = default;
  
  bool Matches(const eowu::Attribute& other) const;
  const std::string& GetKind() const;
  const eowu::AttributeAggregateType& GetComponents() const;
  
  std::size_t Size() const;
  
private:
  std::string kind;
  eowu::AttributeAggregateType components;
};
