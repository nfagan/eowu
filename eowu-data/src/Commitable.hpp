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
  }
}

class eowu::data::Commitable {
  
public:
  Commitable(const Commitable &other);
  
  Commitable();
  ~Commitable() = default;
  
  void Uncommit();
  void Commit();
  void Reset();
  void Serialize(eowu::serialize::ByteArrayType &into);
  
  bool IsCommitted() const;
  
  void Set(const eowu::data::Struct &value);
  void Use(const std::function<void(const eowu::data::Struct&)> &func) const;
  
private:
  mutable std::mutex mut;
  
  std::atomic<bool> is_committed;
  eowu::data::Struct value;
};
