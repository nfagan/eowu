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
  
  struct Coordinate;
}

struct eowu::Coordinate {
  double x;
  double y;
  
  Coordinate();
  ~Coordinate() = default;
};

class eowu::XYSource {
public:
  XYSource() = default;
  ~XYSource() = default;
  
  void Update(eowu::Coordinate coord);
  Coordinate ConsumeLatestSample();
  Coordinate GetLatestSample() const;
  bool NewSampleAvailable();
  
private:
  std::atomic<bool> new_sample_available;
  std::atomic<Coordinate> coordinate;
};
