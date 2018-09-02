//
//  time.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include <eowu-common/time.hpp>

eowu::time::TimePointType eowu::time::now() {
  return std::chrono::high_resolution_clock::now();
}
