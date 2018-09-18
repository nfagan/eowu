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
  class Timer;
  
  namespace time {
    
    struct Timing {
      eowu::time::RenderTiming render;
      const eowu::Timer *task;
      
      Timing(const eowu::Timer *task_timer) : task(task_timer) {}
    };
  }
}
