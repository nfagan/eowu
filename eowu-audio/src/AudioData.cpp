//
//  AudioData.cpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/8/18.
//

#include "AudioData.hpp"
#include "AudioFile.cpp"

eowu::AudioData::AudioData() : sample_rate(0), bit_depth(0), n_samples(0), n_channels(0) {
  //
}

eowu::AudioData eowu::AudioData::FromWAV(const std::string &path) {
  AudioFile<double> audio_file;
  
  audio_file.load(path);
  
  eowu::AudioData audio_data;
  
  audio_data.sample_rate = audio_file.getSampleRate();
  audio_data.bit_depth = audio_file.getBitDepth();
  audio_data.n_samples = audio_file.getNumSamplesPerChannel();
  audio_data.n_channels = audio_file.getNumChannels();
  audio_data.samples = std::move(audio_file.samples);
  
  return audio_data;
}
