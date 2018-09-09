//
//  Commitable.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include "Field.hpp"
#include <vector>
#include <atomic>
#include <mutex>
#include <functional>

namespace eowu {
  namespace data {
    class Commitable;
    class Store;
    
    using CommitablePointerType = std::shared_ptr<eowu::data::Commitable>;
    using CommitableTraversalType = std::function<void(eowu::data::Commitable*)>;
  }
}

class eowu::data::Commitable {
  
public:
  Commitable();
  ~Commitable();
  
  void Commit(const eowu::data::Struct &value, eowu::data::Store &store);
  void Commit(eowu::data::Store &store);
  
  bool IsCommitted() const;
  
  void Set(const eowu::data::Struct &value);
  
  void AddChild(eowu::data::Commitable *other);
  bool HasChild(eowu::data::Commitable *other) const;
  bool HasChildren() const;
  
  void RemoveChild(eowu::data::Commitable *other);
  void ClearChildren();
  
  void Traverse(const CommitableTraversalType &func);
  
  const eowu::data::Struct& GetValue() const;
  
  eowu::data::Commitable* GetRoot() const;
  
private:
  mutable std::recursive_mutex mut;
  
  std::atomic<bool> is_committed;
  eowu::data::Struct value;
  
  eowu::data::Commitable *parent;
  std::vector<eowu::data::Commitable*> children;
};
