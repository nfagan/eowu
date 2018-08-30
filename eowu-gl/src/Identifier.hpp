//
//  Identifier.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <atomic>
#include <functional>
#include <iostream>

namespace eowu {
  class Identifier;
}

class eowu::Identifier {
  friend struct std::hash<eowu::Identifier>;
public:
  Identifier() : numeric_id(eowu::Identifier::get_numeric_id()) {}
  
  bool operator ==(const eowu::Identifier& other) const {
    return numeric_id == other.numeric_id;
  }
  
  bool operator !=(const eowu::Identifier& other) const {
    return !(*this == other);
  }
  
  friend std::ostream& operator<<(std::ostream& os, const eowu::Identifier &id);
  
private:
  eowu::u64 numeric_id;
  
  static eowu::u64 get_numeric_id() {
    static std::atomic<eowu::u64> id = 0;
    eowu::u64 tmp = id.load();
    id.store(tmp + 1);
    return tmp;
  }
  
};

template <>
struct std::hash<eowu::Identifier> {
  std::size_t operator()(const eowu::Identifier& k) const {
    return std::hash<eowu::u64>()(k.numeric_id);
  }
};
