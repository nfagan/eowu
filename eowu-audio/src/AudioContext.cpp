//
//  AudioContext.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioContext.hpp"
#include "Error.hpp"
#include "RtAudio.h"
#include <string>

eowu::AudioContext::AudioContext() :
rt_audio_instance(nullptr), output_stream(nullptr),
source_aggregate(std::make_shared<eowu::AudioSourceAggregate>()), is_initialized(false) {
  //
}

eowu::AudioContext::~AudioContext() {
  CloseOutputStream();
}

void eowu::AudioContext::Initialize() {
  std::string default_message = "Failed to initialize audio context: ";
  
  try {
    rt_audio_instance = std::make_shared<RtAudio>();
  } catch (RtAudioError &e) {
    throw eowu::AudioError(default_message + e.getMessage());
  }
  
  if (rt_audio_instance->getDeviceCount() < 1) {
    throw eowu::AudioError(default_message + " No output devices available.");
  }
  
  is_initialized = true;
}

void eowu::AudioContext::Update() {
  source_aggregate->Update();
}

std::shared_ptr<eowu::AudioSourceAggregate> eowu::AudioContext::GetSourceAggregate() {
  return source_aggregate;
}

std::shared_ptr<eowu::AudioStream> eowu::AudioContext::GetOutputStream() {
  return output_stream;
}

void eowu::AudioContext::StartOutputStream(const eowu::AudioStreamParameters &params) {
  assert_initialized();
  
  output_stream = std::make_shared<eowu::AudioStream>(this, rt_audio_instance, params);
  
  output_stream->open_output();
  output_stream->start();
}

void eowu::AudioContext::CloseOutputStream() {
  if (output_stream == nullptr) {
    return;
  }
  
  output_stream->close();
}

void eowu::AudioContext::assert_initialized() const {
  if (!is_initialized) {
    throw eowu::AudioError("Audio context is not initialized.");
  }
}

eowu::AudioStreamParameters eowu::AudioContext::GetDefaultOutputStreamParameters(bool with_device_id) const {
  eowu::AudioStreamParameters params;
  
  if (!with_device_id) {
    return params;
  }
  
  assert_initialized();
  params.device = rt_audio_instance->getDefaultOutputDevice();
  
  return params;
}
