//
//  test-timer.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-timer.hpp"
#include <iostream>
#include <eowu-state/eowu-state.hpp>

void test_timer_ellapsed() {
  using namespace eowu;
  
  time::TimePointType t1 = time::now();
  time::TimePointType t2;
  
  double limit = 1.0;
  double thresh = 1e-6;
  
  auto timer = Timer();
  
  while (true) {
    t2 = time::now();
    
    if ((t2-t1).count() > limit) {
      break;
    }
  }
  
  auto ellapsed = timer.Elapsed();
  
  if (abs(ellapsed.count() - limit) < thresh) {
    std::cout << "ERROR: Ellapsed time was more than " << limit << " seconds off from actual time." << std::endl;
  } else {
    std::cout << "OK: Ellapsed time was reported accurately." << std::endl;
  }
  
}

void test_timer_run_all() {
  std::cout << "--------" << "timer" << std::endl;
  
  test_timer_ellapsed();
}
