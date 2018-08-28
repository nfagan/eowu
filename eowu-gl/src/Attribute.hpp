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
}

class eowu::Attribute {
public:
  Attribute(const char *kind, const std::vector<eowu::f32> &components);
  ~Attribute() = default;
  
  bool Matches(const eowu::Attribute& other) const;
  const char* GetKind() const;
  const std::vector<eowu::f32>& GetComponents() const;
  
  eowu::u64 Size() const;
  
private:
  const char* kind;
  std::vector<eowu::f32> components;
};
