//
//  Vertex.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include "Attribute.hpp"
#include <eowu-common/types.hpp>
#include <unordered_map>

namespace eowu {
  class Vertex;
}

class eowu::Vertex {
public:
  Vertex() = default;
  ~Vertex() = default;

  void AddAttribute(const eowu::Attribute &attribute);
  bool HasAttribute(const char *name) const;
  const eowu::Attribute* GetAttribute(const char *name) const;
  const std::vector<const char*> GetAttributeKinds() const;
  
  eowu::u64 CountAttributes() const;
  
  eowu::u64 Size() const;
  
  bool Matches(const eowu::Vertex &other) const;
  
private:
  std::unordered_map<const char*, eowu::Attribute> attributes;
};
