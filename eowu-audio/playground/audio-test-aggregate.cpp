//
//  audio-test-aggregate.cpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/9/18.
//

#include "audio-test-aggregate.hpp"
#include "eowu-audio.hpp"

#include "AudioFile.h"
#include "RtAudio.h"
#include <eowu-common/fs.hpp>
#include <cstdlib>

namespace detail {
  static constexpr std::size_t n_channels = 2;
  
  int callback(void *output_buffer,
               void *input_buffer,
               unsigned int n_buffer_frames,
               double stream_time,
               RtAudioStreamStatus status,
               void *user_data)
  {
    double *buffer = (double*) output_buffer;
    
    eowu::AudioSourceAggregate* aggregate = static_cast<eowu::AudioSourceAggregate*>(user_data);
    const std::size_t sz = aggregate->Size();
    
    //  for each frame ...
    for (std::size_t i = 0; i < n_buffer_frames; i++) {
      //  for each channel ...
      for (std::size_t j = 0; j < n_channels; j++) {
        
        double val = 0.0;
        
        //  for each source ...
        for (std::size_t k = 0; k < sz; k++) {
          if (!aggregate->IsActive(k)) {
            continue;
          }
          
          auto *source = aggregate->Get(k);
          
          val += source->GetLatestSample(j);
          
          if (j == n_channels-1) {
            source->Tick();
            
            if (source->Ended()) {
              aggregate->Inactivate(k);
            }
          }
        }
        
        if (val < -1.0) {
          val = -1.0;
        }
        
        if (val > 1.0) {
          val = 1.0;
        }
        
        *buffer++ = val;
      }
    }
    
    return 0;
  }
}

int eowu::audio::main() {
  auto repo_root = eowu::fs::get_eowu_root_directory();
  
  auto piano_path = eowu::fs::full_file({repo_root, "res", "sounds", "piano.wav"});
  auto guitar_path = eowu::fs::full_file({repo_root, "res", "sounds", "guitar.wav"});
  
  eowu::AudioSourceAggregate src;
  
  src.LimitSize(10);
  
  auto piano_buffer_data = eowu::AudioData::FromWavOrAiffFile(piano_path);
  auto guitar_buffer_data = eowu::AudioData::FromWavOrAiffFile(guitar_path);
  
  eowu::AudioBufferSource piano_src(piano_buffer_data);
  eowu::AudioBufferSource guitar_src(guitar_buffer_data);
  
  guitar_src.SetGain(5.0);
  
  auto h1 = src.Push(piano_src);
  auto h2 = src.Push(guitar_src);
  auto h3 = src.Push(piano_src);
  
  RtAudio dac;
  
  if (dac.getDeviceCount() < 1) {
    std::cout << "\nNo audio devices found!\n";
    return 1;
  }
  
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = detail::n_channels;
  parameters.firstChannel = 0;
  
  unsigned int nBufferFrames = 256;
  
  try {
    dac.openStream(&parameters, NULL, RTAUDIO_FLOAT64,
                   44100, &nBufferFrames, &detail::callback, (void *)&src);
    dac.startStream();
  }
  catch (RtAudioError& e) {
    e.printMessage();
    return 1;
  }
  
  while (src.AnyActive() || src.AnyPending()) {
    src.Update();
  }
  
  try {
    // Stop the stream
    dac.stopStream();
  }
  catch (RtAudioError& e) {
    e.printMessage();
  }
  if ( dac.isStreamOpen() ) dac.closeStream();
  
  return 0;
}
