//
//  AudioInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include "SetupResult.hpp"
#include <eowu-audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>

namespace eowu {  
  namespace schema {
    struct Sounds;
  }
  
  namespace init {
    using AudioSourceMapType = std::unordered_map<std::string, std::shared_ptr<eowu::AudioBufferSource>>;
    
    struct AudioInit {
      std::shared_ptr<eowu::AudioContext> audio_context;
      AudioSourceMapType sounds;
    };
    
    using AudioResult = eowu::SetupResult<eowu::init::AudioInit>;
    
    eowu::init::AudioResult initialize_audio_pipeline(const eowu::schema::Sounds &sounds_schema);
    std::shared_ptr<eowu::AudioContext> initialize_audio_context_and_output_stream();
    AudioSourceMapType initialize_buffer_sources(const eowu::schema::Sounds &sounds_schema);
  }
}
