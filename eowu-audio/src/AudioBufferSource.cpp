//
//  AudioBufferSource.cpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#include "AudioBufferSource.hpp"

eowu::AudioBufferSource::AudioBufferSource(std::shared_ptr<eowu::AudioData> data) : audio_data(data), seek_position(0), gain(1.0) {
  //
}

eowu::AudioBufferSource::AudioBufferSource() : eowu::AudioBufferSource::AudioBufferSource(nullptr) {
  //
}

eowu::AudioBufferSource::AudioBufferSource(const eowu::AudioBufferSource &other) :
audio_data(other.audio_data), seek_position(other.seek_position.load()), gain(other.gain.load()) {
  //
}

eowu::AudioBufferSource& eowu::AudioBufferSource::operator=(const eowu::AudioBufferSource &other) {
  audio_data = other.audio_data;
  seek_position = other.seek_position.load();
  gain = other.gain.load();
  
  return *this;
}

void eowu::AudioBufferSource::SetAudioData(std::shared_ptr<eowu::AudioData> data) {
  audio_data = data;
}

void eowu::AudioBufferSource::SetGain(double gain) {
  this->gain.store(gain, std::memory_order_relaxed);
}

unsigned int eowu::AudioBufferSource::GetNumberOfChannels() const {
  if (audio_data == nullptr) {
    return 0;
  }
  
  return audio_data->n_channels;
}

double eowu::AudioBufferSource::GetLatestSample(unsigned int channel) const {
  double silence = 0.0;
  
  if (audio_data == nullptr) {
    return silence;
  }
  
  if (channel >= audio_data->n_channels) {
    return silence;
  }
  
  if (seek_position >= audio_data->n_samples) {
    return silence;
  }
  
  double use_gain = this->gain.load(std::memory_order_relaxed);
  
  return audio_data->samples[channel][seek_position] * use_gain;
}

void eowu::AudioBufferSource::Tick() {
  seek_position++;
}

bool eowu::AudioBufferSource::Ended() const {
  if (audio_data == nullptr) {
    return true;
  }
  
  return seek_position >= audio_data->n_samples;
}
