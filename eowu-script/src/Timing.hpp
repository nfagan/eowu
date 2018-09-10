//
//  Timing.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

#include "TaskTiming.hpp"
#include "RenderTiming.hpp"

namespace eowu {
  namespace time {
    struct Timing {
      eowu::time::TaskTiming task;
      eowu::time::RenderTiming render;
    };
  }
}
