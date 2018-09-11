//
//  LockedResource.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/10/18.
//

#pragma once

#include <mutex>
#include <functional>

namespace eowu {
  template<typename T, typename Mutex = std::mutex>
  class LockedResource;
  
  template<typename T, typename M>
  using LockedResourceCallbackType = std::function<void(LockedResource<T, M>*)>;
}

template<typename T, typename Mutex>
class eowu::LockedResource {
public:
  LockedResource() = default;
  ~LockedResource() = default;
  
  void Use(const LockedResourceCallbackType<T, Mutex> &cb);
  
private:
  mutable Mutex mut;
};

template<typename T, typename Mutex>
void eowu::LockedResource<T, Mutex>::Use(const eowu::LockedResourceCallbackType<T, Mutex> &cb) {
  std::unique_lock<Mutex> lock(mut);
  
  cb(this);
}
