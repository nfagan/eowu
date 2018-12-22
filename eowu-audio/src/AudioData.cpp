//
//  AudioData.cpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/8/18.
//

#include "AudioData.hpp"
#include "Error.hpp"
#include "AudioFile.h"
#include <eowu-common/fs.hpp>

eowu::AudioData::AudioData() : sample_rate(0), bit_depth(0), n_samples(0), n_channels(0) {
  //
}

std::shared_ptr<eowu::AudioData> eowu::AudioData::FromWavOrAiffFile(const std::string &path) {
  AudioFile<double> audio_file;
  
  //  Audio file implementation fails if file is directory.
  if (eowu::fs::directory_exists(path)) {
    throw eowu::AudioLoadError("Failed to load file: " + path + ": is a directory.");
  }
  
  bool success = audio_file.load(path);
  
  if (!success) {
    throw eowu::AudioLoadError("Failed to load file: " + path);
  }
  
  auto audio_data = std::make_shared<eowu::AudioData>();
  
  audio_data->sample_rate = audio_file.getSampleRate();
  audio_data->bit_depth = audio_file.getBitDepth();
  audio_data->n_samples = audio_file.getNumSamplesPerChannel();
  audio_data->n_channels = audio_file.getNumChannels();
  audio_data->samples = std::move(audio_file.samples);
  
  return audio_data;
}
