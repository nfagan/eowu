//
//  main.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test1.hpp"
#include "test_runner_2.hpp"

int main(int argc, char** argv) {
  test_runner_2_run_all();
  test1_run_all();
  
  return 0;
}
