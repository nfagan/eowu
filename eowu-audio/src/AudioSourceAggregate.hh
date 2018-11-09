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
void eowu::AudioSourceAggregateImpl<N>::Push(const eowu::AudioBufferSource &source) {
  std::unique_lock<std::mutex> lock(mut);
  
  std::size_t use_n = size;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (is_active[i]) {
      continue;
    }
    
    //  If there's a queued source, add it
    //  first to the active sources.
    if (queued_sources.size() > 0) {
      sources[i] = queued_sources.front();
      queued_sources.pop_front();
      queued_sources.push_back(source);
    } else {
      sources[i] = source;
    }
    
    is_active[i] = true;
    
    return;
  }
  
  //  all slots are taken, so add it to the queue.
  queued_sources.push_back(source);
}

template<std::size_t N>
void eowu::AudioSourceAggregateImpl<N>::Update() {
  std::unique_lock<std::mutex> lock(mut);
  
  std::size_t use_n = size;
  
  for (std::size_t i = 0; i < use_n; i++) {
    if (is_active[i]) {
      continue;
    }
    
    if (queued_sources.size() > 0) {
      sources[i] = queued_sources.front();
      queued_sources.pop_front();
    } else {
      return;
    }
    
    is_active[i] = true;
  }
}

template<std::size_t N>
bool eowu::AudioSourceAggregateImpl<N>::IsActive(std::size_t at) const {
  return is_active[at];
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
  return queued_sources.size() > 0;
}

template<std::size_t N>
void eowu::AudioSourceAggregateImpl<N>::Inactivate(std::size_t at) {
  is_active[at] = false;
}

template<std::size_t N>
eowu::AudioBufferSource* eowu::AudioSourceAggregateImpl<N>::Get(std::size_t at) {
  return &sources[at];
}
