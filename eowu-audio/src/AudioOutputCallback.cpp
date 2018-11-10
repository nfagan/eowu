//
//  AudioOutputCallback.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioOutputCallback.hpp"
#include "AudioContext.hpp"
#include "AudioSourceAggregate.hpp"

int eowu::audio::output_callback(void *output_buffer,
                                 void *input_buffer,
                                 unsigned int n_buffer_frames,
                                 double stream_time,
                                 RtAudioStreamStatus status,
                                 void *user_data) {
  
  double *buffer = (double*) output_buffer;
  
  eowu::AudioContext *context = static_cast<eowu::AudioContext*>(user_data);
  auto aggregate = context->GetSourceAggregate();
  auto stream = context->GetOutputStream();
  
  const std::size_t sz = aggregate->Size();
  std::size_t n_channels = 2;
  
  if (stream != nullptr) {
    n_channels = stream->CountChannels();
  }
  
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
