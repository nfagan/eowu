//
//  XYSource.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#include "XYSource.hpp"

eowu::Coordinate::Coordinate(double x_, double y_) : x(x_), y(y_) {
  //
}

eowu::Coordinate::Coordinate() : x(0.0), y(0.0) {
  //
}

eowu::XYSource::XYSource() : is_valid(true), new_sample_available(false) {
  //
}

eowu::Coordinate eowu::XYSource::ConsumeLatestSample() {
  auto coord = coordinate.load();
  
  new_sample_available = false;
  
  return coord;
}

eowu::Coordinate eowu::XYSource::GetLatestSample() const {
  return coordinate.load();
}

void eowu::XYSource::Update(eowu::Coordinate coord) {
  coordinate = coord;
  
  new_sample_available = true;
}

bool eowu::XYSource::NewSampleAvailable() const {
  return new_sample_available;
}

void eowu::XYSource::SetIsValid(bool value) {
  is_valid = value;
}

bool eowu::XYSource::IsValid() const {
  return is_valid;
}
