//
//  AudioSourceHandle.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include <mutex>
#include <functional>

namespace eowu {
  class AudioSourceHandle;
  
  template<std::size_t N>
  class AudioSourceAggregateImpl;
  
  class AudioBufferSource;
}

class eowu::AudioSourceHandle {
  template<std::size_t N> friend class eowu::AudioSourceAggregateImpl;
  
public:
  AudioSourceHandle();
  ~AudioSourceHandle() = default;
  
  void Stop();
  bool IsValid() const;
  
private:
  mutable std::mutex mut;
  
  eowu::AudioBufferSource *source;
  
  void set_source(eowu::AudioBufferSource *source);
  void use(const std::function<void(eowu::AudioSourceHandle*)> &cb);
};
