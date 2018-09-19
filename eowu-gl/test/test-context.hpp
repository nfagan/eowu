//
//  test-context.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-gl/eowu-gl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <assert.h>
#include <iostream>

void test_context_initialization();
void test_window_creation();
void test_window_out_of_bounds();

void test_window_out_of_bounds() {
  eowu::ContextManager manager;
  
  manager.Initialize();
  
  try {
    auto win = manager.OpenWindow(1);
    std::cout << "ERROR: Allowed window to open on out of bounds monitor" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "OK: Out of bounds window opening failed with message: " << e.what() << std::endl;
  }
}

void test_window_creation() {
	eowu::ContextManager manager;
  
  manager.Initialize();
  
  try {
    auto win = manager.OpenWindow();
    std::cout << "OK: Window opened succesfully" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Window opening failed with message: " << e.what() << std::endl;
  }
}

void test_context_initialization() {
  
	eowu::ContextManager manager;
  
  try {
    manager.Initialize();
    
    assert(manager.IsInitialized());
    
    std::cout << "OK: Context initialized successfully" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Context failed to initialize with message: ." << e.what() << std::endl;
  }
  
}

void test_context_run_all() {
  std::cout << "--------" << "context" << std::endl;
  
  test_context_initialization();
  test_window_creation();
  test_window_out_of_bounds();
}
