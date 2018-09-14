//
//  XYTarget.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#include "XYTarget.hpp"
#include "XYSource.hpp"

eowu::XYTarget::XYTarget() : source(nullptr) {
  //
}

eowu::XYTarget::XYTarget(eowu::XYSource *source_) : source(source_) {
  //
}

void eowu::XYTarget::Update() {
  std::unique_lock<std::mutex> lock(mut);
  
  if (source == nullptr) {
    return;
  }
  
  auto coord = source->GetLatestSample();
  
  if (bounds_function(coord)) {
    
  }
}

void eowu::XYTarget::SetSource(eowu::XYSource *source) {
  std::unique_lock<std::mutex> lock(mut);
  
  this->source = source;
}

void eowu::XYTarget::SetBoundsFunction(const std::function<bool(eowu::Coordinate)> &func) {
  std::unique_lock<std::mutex> lock(mut);
  
  bounds_function = func;
}
