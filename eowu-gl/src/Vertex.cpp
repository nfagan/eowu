//
//  Vertex.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/27/18.
//

#include "Vertex.hpp"
#include <cstring>
#include <utility>

void eowu::Vertex::AddAttribute(const eowu::Attribute &attribute) {
  attributes.insert(std::make_pair(attribute.GetKind(), attribute));
}

bool eowu::Vertex::HasAttribute(const char *name) const {
  const auto& it = attributes.find(name);
  return it != attributes.end();
}

eowu::u64 eowu::Vertex::CountAttributes() const {
  return attributes.size();
}

const eowu::Attribute* eowu::Vertex::GetAttribute(const char *name) const {
  if (!HasAttribute(name)) {
    return nullptr;
  }
  
  return &attributes.at(name);
}

const std::vector<const char*> eowu::Vertex::GetAttributeKinds() const {
  std::vector<const char*> res;
  
  for (const auto& it : attributes) {
    res.push_back(it.first);
  }
  
  return res;
}

eowu::u64 eowu::Vertex::Size() const {
  eowu::u64 sz = 0;
  
  for (const auto& it : attributes) {
    sz += it.second.Size();
  }
  
  return sz;
}

bool eowu::Vertex::Matches(const eowu::Vertex &other) const {
  if (attributes.size() != other.attributes.size()) {
    return false;
  }
  
  for (const auto& it : attributes) {
    const auto own_attribute = it.second;
    const auto own_kind = own_attribute.GetKind();
    
    if (!other.HasAttribute(own_kind)) {
      return false;
    }
    
    const auto other_attribute = other.GetAttribute(own_kind);
    
    if (!other_attribute->Matches(own_attribute)) {
      return false;
    }
  }
  
  return true;
}
