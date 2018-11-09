//
//  AudioSourceAggregate.hpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#pragma once

#include "AudioBufferSource.hpp"
#include <array>
#include <deque>
#include <atomic>
#include <mutex>

namespace eowu {
  template<std::size_t N>
  class AudioSourceAggregateImpl;
  
  using AudioSourceAggregate = AudioSourceAggregateImpl<10>;
}

template<std::size_t N>
class eowu::AudioSourceAggregateImpl {
public:
  AudioSourceAggregateImpl();
  ~AudioSourceAggregateImpl() = default;
  
  void Push(const eowu::AudioBufferSource &source);
  eowu::AudioBufferSource* Get(std::size_t at);
  
  bool LimitSize(std::size_t to_size);
  
  std::size_t Size() const;
  std::size_t Capacity() const;
  
  void Update();
  
  void Inactivate(std::size_t at);
  bool IsActive(std::size_t at) const;
  
  bool AnyActive() const;
  bool AnyPending() const;
private:
  mutable std::mutex mut;
  
  std::array<eowu::AudioBufferSource, N> sources;
  std::array<std::atomic<bool>, N> is_active;
  
  std::deque<eowu::AudioBufferSource> queued_sources;
  
  std::atomic<std::size_t> size;
};

#include "AudioSourceAggregate.hh"
