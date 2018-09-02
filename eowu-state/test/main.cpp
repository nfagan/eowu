//
//  main.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-state.hpp"
#include "test-timer.hpp"

int main(int argc, char** argv) {
  test_state_run_all();
  test_timer_run_all();
  
  return 0;
}
