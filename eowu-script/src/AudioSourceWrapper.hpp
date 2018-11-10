//
//  AudioSourceWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include <eowu-audio.hpp>
#include <memory>

struct lua_State;

namespace eowu {
  class AudioSourceWrapper;
  class AudioSourceHandleWrapper;
}

class eowu::AudioSourceWrapper {
  
public:
  AudioSourceWrapper(std::shared_ptr<eowu::AudioSourceAggregate> source_aggregate,
                     std::shared_ptr<eowu::AudioBufferSource> buffer_source);
  ~AudioSourceWrapper() = default;
  
  eowu::AudioSourceHandleWrapper Play();
  double SlotsRemaining() const;
  double Capacity() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::AudioSourceAggregate> source_aggregate;
  std::shared_ptr<eowu::AudioBufferSource> buffer_source;
};
