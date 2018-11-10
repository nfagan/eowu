//
//  AudioInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioInit.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/logging.hpp>

eowu::init::AudioResult eowu::init::initialize_audio_pipeline(const eowu::schema::Sounds &sounds_schema) {
  eowu::init::AudioResult result;
  
  try {
    result.result.audio_context = initialize_audio_context_and_output_stream();
  } catch (const std::runtime_error &e) {
    result.status.context = "Audio::Initialize";
    result.status.message = e.what();
    
    return result;
  }
  
  try {
    result.result.sounds = initialize_buffer_sources(sounds_schema);
  } catch (const std::runtime_error &e) {
    result.status.context = "Audio::LoadSounds";
    result.status.message = e.what();
    
    return result;
  }
  
  result.status.success = true;
  
  return result;
}

std::shared_ptr<eowu::AudioContext> eowu::init::initialize_audio_context_and_output_stream() {
  auto audio_context = std::make_shared<eowu::AudioContext>();
  
  audio_context->Initialize();
  audio_context->StartOutputStream(audio_context->GetDefaultOutputStreamParameters());
  
  return audio_context;
}

eowu::init::AudioSourceMapType eowu::init::initialize_buffer_sources(const eowu::schema::Sounds &sounds_schema) {
  eowu::init::AudioSourceMapType result;
  
  const auto &mapping = sounds_schema.mapping;
  
  std::unordered_map<std::string, std::shared_ptr<eowu::AudioData>> processed_audio_data;
  
  for (const auto &it : mapping) {
    const std::string &key = it.first;
    const std::string &filename = it.second;
    
    std::shared_ptr<eowu::AudioData> audio_data = nullptr;
    
    if (processed_audio_data.count(filename) > 0) {
      audio_data = processed_audio_data.at(filename);
    } else {
      EOWU_LOG_INFO("init::load_buffer_sources: Loading '" + filename + "'");
      
      audio_data = eowu::AudioData::FromWavOrAiffFile(filename);
      processed_audio_data.emplace(filename, audio_data);
    }
    
    auto buffer_source = std::make_shared<eowu::AudioBufferSource>(audio_data);
    
    result.emplace(key, buffer_source);
  }
  
  EOWU_LOG_INFO("init::load_buffer_sources: Done loading.");
  
  return result;
}
