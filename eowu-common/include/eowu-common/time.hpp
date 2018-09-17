//
//  time.hpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <chrono>

namespace eowu {
  namespace time {
    using TimePointType = std::chrono::high_resolution_clock::time_point;
    using DurationType = std::chrono::duration<double>;
    
    TimePointType now();
    DurationType zero();
  }
}
