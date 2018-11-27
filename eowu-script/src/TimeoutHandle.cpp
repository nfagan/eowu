//
//  TimeoutHandle.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/17/18.
//

#include "TimeoutHandle.hpp"
#include "TimeoutWrapper.hpp"

eowu::TimeoutHandle::TimeoutHandle(eowu::TimeoutWrapper *wrapper) :
timeout(wrapper) {
  //
}

void eowu::TimeoutHandle::Invalidate() {
  timeout = nullptr;
}

void eowu::TimeoutHandle::Cancel() {
  auto timeout_ptr = timeout.load();
  
  if (timeout_ptr == nullptr) {
    return;
  }
  
  timeout_ptr->Cancel();
}
