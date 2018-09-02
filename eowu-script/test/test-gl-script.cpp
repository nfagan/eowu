//
//  test-gl-script.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-gl-script.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <iostream>

void test_gl_script_run_all() {
  
  auto manager = eowu::ContextManager();
  manager.Initialize();
  
  auto win = manager.OpenWindow(600, 600);
  auto win2 = manager.OpenWindow();
  auto win3 = manager.OpenWindow();
  
  auto t1 = std::chrono::high_resolution_clock::now();
  
  while (true) {
    auto t2 = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> res = t2-t1;
    
    if (res.count() > 4) {
      break;
    }
  }
  
}
