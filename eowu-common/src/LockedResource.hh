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

template<typename T, typename M>
void eowu::LockedResource<T, M>::UnsafeUseUnlocked(const std::function<void(T*)> &cb) {
  cb(&value);
}

template<typename T, typename M>
void eowu::LockedResource<T, M>::UnsafeUseUnlocked(const std::function<void(const T&)> &cb) const {
  cb(value);
}

template<typename T, typename M>
void eowu::LockedResource<T, M>::UnsafeUseUnlocked(const std::function<void(T&)> &cb) {
  cb(value);
}

template<typename T, typename M>
std::thread::id eowu::LockedResource<T, M>::GetLockingThreadId() const {
  return locking_thread_id.load();
}

