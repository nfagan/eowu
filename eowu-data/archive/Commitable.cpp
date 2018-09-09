//
//  Commitable.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Commitable.hpp"
#include "Error.hpp"
#include <cstddef>

eowu::data::Commitable::Commitable() :
is_committed(false), parent(nullptr) {
  //
}

eowu::data::Commitable::~Commitable() {
  if (parent) {
    parent->RemoveChild(this);
  }
}

bool eowu::data::Commitable::IsCommitted() const {
  return is_committed;
}

void eowu::data::Commitable::AddChild(eowu::data::Commitable *other) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  if (HasChild(other)) {
    return;
  }
  
  children.push_back(other);
  
  if (other->parent != nullptr) {
    other->parent->RemoveChild(other);
  }
  
  other->parent = this;
}

bool eowu::data::Commitable::HasChild(eowu::data::Commitable *other) const {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  for (const auto &child : children) {
    if (child == other) {
      return true;
    }
  }
  
  return false;
}

bool eowu::data::Commitable::HasChildren() const {
  std::unique_lock<std::recursive_mutex> lock(mut);
  return children.size() > 0;
}

const eowu::data::Struct& eowu::data::Commitable::GetValue() const {
  return value;
}

eowu::data::Commitable* eowu::data::Commitable::GetRoot() const {
  eowu::data::Commitable *root = parent;
  
  while (root) {
    root = parent->parent;
  }
  
  return root;
}

void eowu::data::Commitable::Traverse(const CommitableTraversalType &func) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  func(this);
  
  for (const auto &child : children) {
    func(child);
  }
}

void eowu::data::Commitable::RemoveChild(eowu::data::Commitable *other) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  std::size_t N = children.size();
  
  for (std::size_t i = 0; i < N; i++) {
    if (children[i] == other) {
      children.erase(children.begin() + i);
      other->parent = nullptr;
      return;
    }
  }
}

void eowu::data::Commitable::ClearChildren() {
  std::unique_lock<std::recursive_mutex> lock(mut);
  children.clear();
}

void eowu::data::Commitable::Set(const eowu::data::Struct &value) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  this->value = value;
}

void eowu::data::Commitable::Commit(eowu::data::Store &store) {
  if (IsCommitted()) {
    throw eowu::AlreadyCommittedError("Attempted to commit an already committed value.");
  }
  
  //
}
