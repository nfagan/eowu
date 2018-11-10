//
//  AudioSourceAggregate.hh
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#pragma once

#include <algorithm>

template<std::size_t N>
eowu::AudioSourceAggregateImpl<N>::AudioSourceAggregateImpl() {
  std::fill(is_active.begin(), is_active.end(), false);
  std::fill(handles.begin(), handles.end(), nullptr);
  
  size = N;
}

template<std::size_t N>
std::size_t eowu::AudioSourceAggregateImpl<N>::Size() const {
  return size;
}

template<std::size_t N>
std::size_t eowu::AudioSourceAggregateImpl<N>::Capacity() const {
  return N;
}

template<std::size_t N>
bool eowu::AudioSourceAggregateImpl<N>::LimitSize(std::size_t to_size) {
  bool success = true;
  
  if (to_size > N) {
    to_size = N;
    success = false;
  }
  
  size = to_size;
  
  return success;
}

template<std::size_t N>
std::shared_ptr<eowu::AudioSourceHandle> eowu::AudioSourceAggregateImpl<N>::Push(const eowu::AudioBufferSource &source) {
  std::unique_lock<std::mutex> lock(mut);
  queued_sources.push_back(source);
  
  auto handle = std::make_shared<eowu::AudioSourceHandle>();
  handle->set_source(&queued_sources.back());
  
  queued_handles.push_back(handle);
  
  return handle;
}

template<std::size_t N>
std::shared_ptr<eowu::AudioSourceHandle> eowu::AudioSourceAggregateImpl<N>::PushSync(const eowu::AudioBufferSource &source) {
  
  while (true) {
    std::size_t n_pending = CountPending();
    std::size_t n_inactive = CountInactive();
    
    //  At least one free slot in which source can be inserted.
    if (n_inactive > n_pending) {
      break;
    }
  }
  
  return Push(source);
}

template<std::size_t N>
void eowu::AudioSourceAggregateImpl<N>::Update() {
  std::unique_lock<std::mutex> lock(mut);
  
  std::size_t use_n = size;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (queued_sources.size() == 0) {
      return;
    }
    
    if (is_active[i]) {
      continue;
    }
    
    const auto &queued_source = queued_sources.front();
    const auto &queued_handle = queued_handles.front();
    auto current_handle = handles[i];
    
    if (current_handle != nullptr) {
      current_handle->set_source(nullptr);
    }
    
    queued_handle->use([&] (auto *handle) -> void {
      sources[i] = queued_source;
      handles[i] = queued_handle;
      
      handle->source = &sources[i];
    });
    
    queued_sources.pop_front();
    queued_handles.pop_front();
    
    is_active[i] = true;
  }
}

template<std::size_t N>
bool eowu::AudioSourceAggregateImpl<N>::IsActive(std::size_t at) const {
  return is_active[at];
}

template<std::size_t N>
std::size_t eowu::AudioSourceAggregateImpl<N>::CountActive() const {
  std::size_t use_n = size;
  std::size_t n = 0;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (is_active[i]) {
      n++;
    }
  }
  
  return n;
}

template<std::size_t N>
std::size_t eowu::AudioSourceAggregateImpl<N>::CountInactive() const {
  std::size_t use_n = size;
  std::size_t n = 0;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (!is_active[i]) {
      n++;
    }
  }
  
  return n;
}

template<std::size_t N>
std::size_t eowu::AudioSourceAggregateImpl<N>::CountPending() const {
  std::unique_lock<std::mutex> lock(mut);
  return queued_sources.size();
}

template<std::size_t N>
bool eowu::AudioSourceAggregateImpl<N>::AnyActive() const {
  std::size_t use_n = size;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (is_active[i]) {
      return true;
    }
  }
  
  return false;
}

template<std::size_t N>
bool eowu::AudioSourceAggregateImpl<N>::AnyPending() const {
  return CountPending() > 0;
}

template<std::size_t N>
void eowu::AudioSourceAggregateImpl<N>::Inactivate(std::size_t at) {
  is_active[at] = false;
}

template<std::size_t N>
eowu::AudioBufferSource* eowu::AudioSourceAggregateImpl<N>::Get(std::size_t at) {
  return &sources[at];
}
