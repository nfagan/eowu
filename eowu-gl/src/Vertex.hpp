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
#include <cstddef>

namespace eowu {
  class Vertex;
}

class eowu::Vertex {
public:
  Vertex() = default;
  ~Vertex() = default;

  void AddAttribute(const eowu::Attribute &attribute);
  bool HasAttribute(const std::string &name) const;
  const eowu::Attribute* GetAttribute(const std::string &name) const;
  std::vector<std::string> GetAttributeKinds() const;
  
  std::size_t CountAttributes() const;
  
  std::size_t Size() const;
  
  bool Matches(const eowu::Vertex &other) const;
  
private:
  std::unordered_map<std::string, eowu::Attribute> attributes;
};
