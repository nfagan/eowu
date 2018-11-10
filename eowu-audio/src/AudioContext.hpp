//
//  AudioContext.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include "AudioStream.hpp"
#include "AudioSourceAggregate.hpp"
#include <memory>

class RtAudio;

namespace eowu {
  class AudioContext;
}

class eowu::AudioContext {
public:
  AudioContext();
  ~AudioContext();
  
  void Initialize();
  void StartOutputStream(const eowu::AudioStreamParameters &params);
  void CloseOutputStream();
  void Update();
  
  std::shared_ptr<eowu::AudioSourceAggregate> GetSourceAggregate();
  std::shared_ptr<eowu::AudioStream> GetOutputStream();
  
  eowu::AudioStreamParameters GetDefaultOutputStreamParameters(bool with_device_id = true) const;
private:
  std::shared_ptr<RtAudio> rt_audio_instance;
  std::shared_ptr<eowu::AudioStream> output_stream;
  std::shared_ptr<eowu::AudioSourceAggregate> source_aggregate;
  
  bool is_initialized;
  
  void assert_initialized() const;
};
