//
//  AudioSourceHandleWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include <eowu-audio.hpp>

struct lua_State;

namespace eowu {
  class AudioSourceHandleWrapper;
}

class eowu::AudioSourceHandleWrapper {
public:
  AudioSourceHandleWrapper(std::shared_ptr<eowu::AudioSourceHandle> source_handle);
  ~AudioSourceHandleWrapper() = default;
  
  void Stop();
  bool IsValid() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::AudioSourceHandle> source_handle;
};
