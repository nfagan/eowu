//
//  XYSource.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#include "XYSource.hpp"

eowu::XYSource::Coordinate::Coordinate() : x(0.0), y(0.0) {
  //
}

eowu::XYSource::Coordinate eowu::XYSource::ConsumeLatestSample() {
  auto coord = coordinate.load();
  
  new_sample_available = false;
  
  return coord;
}

eowu::XYSource::Coordinate eowu::XYSource::GetLatestSample() const {
  return coordinate.load();
}

void eowu::XYSource::Update(eowu::XYSource::Coordinate coord) {
  coordinate = coord;
  
  new_sample_available = true;
}

bool eowu::XYSource::NewSampleAvailable() {
  return new_sample_available;
}