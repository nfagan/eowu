//
//  XYSource.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#pragma once

#include <atomic>

namespace eowu {
  class XYSource;
}

class eowu::XYSource {
public:
  struct Coordinate {
    double x;
    double y;
    
    Coordinate();
    ~Coordinate() = default;
  };
  
  XYSource() = default;
  ~XYSource() = default;
  
  void Update(eowu::XYSource::Coordinate coord);
  Coordinate ConsumeLatestSample();
  Coordinate GetLatestSample() const;
  bool NewSampleAvailable();
  
private:
  std::atomic<bool> new_sample_available;
  std::atomic<Coordinate> coordinate;
};
