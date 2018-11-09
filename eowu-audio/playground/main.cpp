//
//  main.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/8/18.
//

#include "audio-test-playback.hpp"
#include "audio-test-wav-player.hpp"
#include "audio-test-aggregate.hpp"
#include <stdio.h>
#include <iostream>

int main(int argc, char **argv) {
//  int res = audio::main_wav();
  int res = eowu::audio::main();
  
  std::cout << "OK" << std::endl;
  
  return res;
}
