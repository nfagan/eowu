//
//  AudioStream.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioStream.hpp"
#include "AudioOutputCallback.hpp"
#include "Error.hpp"
#include <eowu-common/logging.hpp>

RtAudio::StreamParameters eowu::AudioStreamParameters::GetRtStreamParameters() {
  RtAudio::StreamParameters rt_params;
  
  rt_params.deviceId = device;
  rt_params.nChannels = n_channels;
  rt_params.firstChannel = 0;
  
  return rt_params;
}

eowu::AudioStream::AudioStream(const eowu::AudioContext *context,
                               std::shared_ptr<RtAudio> rt_audio,
                               const eowu::AudioStreamParameters &params) :
audio_context(context), rt_audio_instance(rt_audio), stream_parameters(params), is_open(false) {
  rt_stream_parameters = stream_parameters.GetRtStreamParameters();
}

unsigned int eowu::AudioStream::CountChannels() const {
  return stream_parameters.n_channels;
}

eowu::AudioStream::~AudioStream() {
  try {
    stop();
  } catch (const eowu::AudioError &e) {
    EOWU_LOG_WARN(e.what());
  }
  
  try {
    close();
  } catch (const eowu::AudioError &e) {
    EOWU_LOG_WARN(e.what());
  }
}

void eowu::AudioStream::open_output() {
  if (is_open) {
    return;
  }
  
  try {
    rt_audio_instance->openStream(&rt_stream_parameters,
                                  NULL,
                                  RTAUDIO_FLOAT64,
                                  stream_parameters.sample_rate,
                                  &stream_parameters.buffer_size,
                                  &eowu::audio::output_callback,
                                  (void*)audio_context);
  } catch (const RtAudioError &e) {
    throw eowu::AudioError("Failed to open stream: " + e.getMessage());
  }
  
  is_open = true;
}

void eowu::AudioStream::start() {
  if (!is_open) {
    return;
  }
  
  try {
    rt_audio_instance->startStream();
  } catch (const RtAudioError &e) {
    throw eowu::AudioError("Failed to start stream: " + e.getMessage());
  }
}

void eowu::AudioStream::stop() {
  if (!is_open) {
    return;
  }
  
  try {
    rt_audio_instance->stopStream();
  } catch (const RtAudioError& e) {
    EOWU_LOG_WARN("Failed to close stream: " + e.getMessage());
  }
}

void eowu::AudioStream::close() {
  if (!is_open) {
    return;
  }
  
  if (rt_audio_instance->isStreamOpen()) {
    rt_audio_instance->closeStream();
  }
  
  is_open = false;
}

//
//  Audio stream parameters
//

eowu::AudioStreamParameters::AudioStreamParameters() :
n_channels(2), sample_rate(44100), buffer_size(256), device(0) {
  //
}
