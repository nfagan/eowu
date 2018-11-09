//
//  AudioData.hpp
//  eowu-audio
//
//  Created by Nick Fagan on 11/8/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <vector>
#include <memory>

namespace eowu {
  struct AudioData;
}

struct eowu::AudioData {
  AudioData();
  ~AudioData() = default;
  
  eowu::u32 sample_rate;
  int bit_depth;
  int n_samples;
  int n_channels;
  
  std::vector<std::vector<double>> samples;
  
  static std::shared_ptr<eowu::AudioData> FromWavOrAiffFile(const std::string &path);
};
