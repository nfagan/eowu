//
//  AudioSourceHandle.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioSourceHandle.hpp"
#include "AudioBufferSource.hpp"
#include <iostream>

eowu::AudioSourceHandle::AudioSourceHandle() : source(nullptr) {
  //
}

bool eowu::AudioSourceHandle::IsValid() const {
  std::unique_lock<std::mutex> lock(mut);
  
  return source != nullptr;
}

void eowu::AudioSourceHandle::Stop() {
  std::unique_lock<std::mutex> lock(mut);
  
  if (source == nullptr) {
    return;
  }
  
  source->Stop();
}

void eowu::AudioSourceHandle::set_source(eowu::AudioBufferSource *source) {
  std::unique_lock<std::mutex> lock(mut);
  
  this->source = source;
}

void eowu::AudioSourceHandle::use(const std::function<void(eowu::AudioSourceHandle*)> &cb) {
  std::unique_lock<std::mutex> lock(mut);
  
  cb(this);
}
