//
//  audio-test-wav-player.cpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/8/18.
//

#include "audio-test-wav-player.hpp"
#include "AudioFile.h"
#include "RtAudio.h"
#include <eowu-common/fs.hpp>
#include <cstdlib>

// Two-channel sawtooth wave generator.
namespace wav {
  struct UserData {
    AudioFile<double> *audio_file;
    unsigned int offset;
    
    UserData() : offset(0) {}
  };
  
  int saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
          double streamTime, RtAudioStreamStatus status, void *userData )
  {
    unsigned int i, j;
    double *buffer = (double *) outputBuffer;
    
    UserData *user_data = static_cast<UserData*>(userData);
    
    std::size_t max_n = user_data->audio_file->samples[0].size();
    
    if ( status )
      std::cout << "Stream underflow detected!" << std::endl;
    // Write interleaved audio data.
    for ( i=0; i<nBufferFrames; i++ ) {
      for ( j=0; j<2; j++ ) {
        std::size_t index = i + user_data->offset;
        double val;
        
        if (index >= max_n) {
          val = 0.0;
        } else {
          val = user_data->audio_file->samples[j][index];
        }
        
        *buffer++ = val;
      }
    }
    
    user_data->offset += nBufferFrames;
    
    return 0;
  }
}

int audio::main_wav() {
  auto repo_root = eowu::fs::get_eowu_root_directory();
  
  auto path = eowu::fs::full_file({repo_root, "res", "sounds", "piano.wav"});
  
  AudioFile<double> audioFile;
  
  std::cout << "Loading sound ..." << std::endl;
  audioFile.load(path);
  std::cout << "Done loading sound ..." << std::endl;
  
  int sampleRate = audioFile.getSampleRate();
  int bitDepth = audioFile.getBitDepth();
  
  unsigned int numSamples = audioFile.getNumSamplesPerChannel();
  unsigned int nBufferFrames = 256;
  
  // or, just use this quick shortcut to print a summary to the console
  audioFile.printSummary();
  
  RtAudio dac;
  
  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  
  wav::UserData user_data;
  user_data.audio_file = &audioFile;
  
  try {
    dac.openStream(&parameters, NULL, RTAUDIO_FLOAT64,
                   sampleRate, &nBufferFrames, &wav::saw, (void *)&user_data);
    dac.startStream();
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
  
  char input;
  std::cout << "\nPlaying ... press <enter> to quit.\n";
  std::cin.get( input );
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
