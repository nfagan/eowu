//
//  AudioStream.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include "RtAudio.h"
#include <memory>

namespace eowu {
  class AudioContext;
  class AudioStream;
  struct AudioStreamParameters;
}

struct eowu::AudioStreamParameters {
  unsigned int n_channels;
  unsigned int sample_rate;
  unsigned int buffer_size;
  unsigned int device;
  
  RtAudio::StreamParameters GetRtStreamParameters();
  
  AudioStreamParameters();
  ~AudioStreamParameters() = default;
};

class eowu::AudioStream {
  friend class eowu::AudioContext;
  
public:
  AudioStream(const eowu::AudioContext *context,
              std::shared_ptr<RtAudio> rt_audio_instance,
              const eowu::AudioStreamParameters &params);
  ~AudioStream();
  
  unsigned int CountChannels() const;
private:
  void open_output();
  void stop();
  void close();
  void start();
  
private:
  const eowu::AudioContext *audio_context;
  std::shared_ptr<RtAudio> rt_audio_instance;
  eowu::AudioStreamParameters stream_parameters;
  RtAudio::StreamParameters rt_stream_parameters;
  
  bool is_open;
};


