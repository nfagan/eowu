//
//  AudioSourceAggregate.hpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#pragma once

#include "AudioBufferSource.hpp"
#include "AudioSourceHandle.hpp"
#include <array>
#include <deque>
#include <atomic>
#include <mutex>
#include <memory>

namespace eowu {
  template<std::size_t N>
  class AudioSourceAggregateImpl;
  
  using AudioSourceAggregate = AudioSourceAggregateImpl<100>;
}

template<std::size_t N>
class eowu::AudioSourceAggregateImpl {
public:
  AudioSourceAggregateImpl();
  ~AudioSourceAggregateImpl() = default;
  
  std::shared_ptr<eowu::AudioSourceHandle> Push(const eowu::AudioBufferSource &source);
  std::shared_ptr<eowu::AudioSourceHandle> PushSync(const eowu::AudioBufferSource &source);
  
  eowu::AudioBufferSource* Get(std::size_t at);
  
  bool LimitSize(std::size_t to_size);
  
  std::size_t Size() const;
  std::size_t Capacity() const;
  
  void Update();
  
  void Inactivate(std::size_t at);
  bool IsActive(std::size_t at) const;
  std::size_t CountActive() const;
  std::size_t CountInactive() const;
  std::size_t CountPending() const;
  
  bool AnyActive() const;
  bool AnyPending() const;
private:
  mutable std::mutex mut;
  
  std::array<eowu::AudioBufferSource, N> sources;
  std::array<std::shared_ptr<eowu::AudioSourceHandle>, N> handles;
  std::array<std::atomic<bool>, N> is_active;
  
  std::deque<eowu::AudioBufferSource> queued_sources;
  std::deque<std::shared_ptr<eowu::AudioSourceHandle>> queued_handles;
  
  std::atomic<std::size_t> size;
};

#include "AudioSourceAggregate.hh"
