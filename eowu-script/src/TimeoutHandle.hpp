//
//  TimeoutHandle.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/17/18.
//

#pragma once

#include <atomic>

namespace eowu {
  class TimeoutHandle;
  class TimeoutWrapper;
}

class eowu::TimeoutHandle {
public:
  //  Shouldn't be shared_ptr to avoid circular reference
  TimeoutHandle(eowu::TimeoutWrapper *timeout);
  
  void Invalidate();
  void Cancel();
  
private:
  std::atomic<eowu::TimeoutWrapper*> timeout;
};
