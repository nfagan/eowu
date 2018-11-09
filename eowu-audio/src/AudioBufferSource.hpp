//
//  AudioBufferSource.hpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#pragma once

#include "AudioData.hpp"
#include <cstdint>
#include <atomic>
#include <memory>

namespace eowu {
  class AudioBufferSource;
}

class eowu::AudioBufferSource {
public:
  AudioBufferSource();
  AudioBufferSource(const eowu::AudioBufferSource &other);
  eowu::AudioBufferSource& operator=(const eowu::AudioBufferSource &other);
  
  AudioBufferSource(std::shared_ptr<eowu::AudioData> data);
  ~AudioBufferSource() = default;
  
  void SetAudioData(std::shared_ptr<eowu::AudioData> data);
  void SetGain(double gain);
  
  unsigned int GetNumberOfChannels() const;
  double GetLatestSample(unsigned int channel) const;
  
  bool Ended() const;
  void Tick();
  
private:
  std::shared_ptr<eowu::AudioData> audio_data;
  
  std::atomic<std::size_t> seek_position;
  std::atomic<double> gain;
};
