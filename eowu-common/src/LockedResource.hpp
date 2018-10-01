//
//  LockedResource.hpp
//  eowu
//
//  Created by Nick Fagan on 9/27/18.
//

#pragma once

#include <functional>
#include <mutex>

namespace eowu {
  template<typename T, typename Mutex = std::mutex>
  class LockedResource;
}

template<typename T, typename Mutex>
class eowu::LockedResource {
public:
  LockedResource(const T &value);
  LockedResource() = default;
  ~LockedResource() = default;
  
  void Use(const std::function<void(T*)> &cb);
  void Use(const std::function<void(const T&)> &cb) const;
  void Use(const std::function<void(T&)> &cb);
  
private:
  mutable Mutex mut;
  
  T value;
};

#include "LockedResource.hh"
