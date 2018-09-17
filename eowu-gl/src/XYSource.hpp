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
  
  Coordinate(double x, double y);
  Coordinate();
  ~Coordinate() = default;
};

class eowu::XYSource {
public:
  XYSource();
  ~XYSource() = default;
  
  void Update(eowu::Coordinate coord);
  
  Coordinate ConsumeLatestSample();
  Coordinate GetLatestSample() const;
  
  void SetIsValid(bool value);
  
  bool NewSampleAvailable() const;
  bool IsValid() const;
  
private:
  std::atomic<bool> is_valid;
  std::atomic<bool> new_sample_available;
  
  std::atomic<Coordinate> coordinate;
};
