//
//  AudioOutputCallback.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#pragma once

#include "RtAudio.h"

namespace eowu {
  namespace audio {
    int output_callback(void *output_buffer,
                        void *input_buffer,
                        unsigned int n_buffer_frames,
                        double stream_time,
                        RtAudioStreamStatus status,
                        void *user_data);
  }
}
