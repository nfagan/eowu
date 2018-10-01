//
//  LockedResource.hh
//  eowu
//
//  Created by Nick Fagan on 9/27/18.
//

#pragma once

template<typename T, typename M>
eowu::LockedResource<T, M>::LockedResource(const T &val) : value(val) {
  //
}

template<typename T, typename M>
void eowu::LockedResource<T, M>::Use(const std::function<void(T*)> &cb) {
  std::unique_lock<M> lock(mut);
  
  cb(&value);
}

template<typename T, typename M>
void eowu::LockedResource<T, M>::Use(const std::function<void(const T&)> &cb) const {
  std::unique_lock<M> lock(mut);
  
  cb(value);
}

template<typename T, typename M>
void eowu::LockedResource<T, M>::Use(const std::function<void(T&)> &cb) {
  std::unique_lock<M> lock(mut);
  
  cb(value);
}
