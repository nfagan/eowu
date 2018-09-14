//
//  XYTarget.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#pragma once

#include "XYSource.hpp"
#include <eowu-common/Timer.hpp>
#include <functional>
#include <mutex>

namespace eowu {
  class XYTarget;
}

class eowu::XYTarget {
public:
  XYTarget();
  XYTarget(eowu::XYSource *source);
  ~XYTarget() = default;
  
  void Update();
  void SetSource(eowu::XYSource *source);
  void SetBoundsFunction(const std::function<bool(eowu::Coordinate)> &func);
  
private:
  mutable std::mutex mut;
  
  eowu::XYSource *source;
  std::function<bool(eowu::Coordinate)> bounds_function;
  eowu::Timer timer;
};
