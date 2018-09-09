//
//  test.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/4/18.
//

#pragma once

#include <iostream>

#define EOWU_ASSERT_TRUE(cond, success_message, fail_message) \
  if (!(cond)) { \
    std::cout << "FAILED: " << fail_message << std::endl << std::endl; \
  } else { \
    std::cout << "OK: " << success_message << std::endl << std::endl; \
  }

#define EOWU_ASSERT_THROWS(expr, success_message, fail_message) \
  try { \
    expr \
    std::cout << "FAILED: " << fail_message << std::endl << std::endl; \
  } catch (const std::exception &e) { \
    std::cout << "OK: " << fail_message << e.what() << std::endl << std::endl; \
  }

#define EOWU_ASSERT_NOTHROW(expr, success_message, fail_message) \
  try { \
    expr; \
    std::cout << "OK: " << success_message << std::endl << std::endl; \
  } catch (const std::exception &e) { \
    std::cout << "FAIL: " << fail_message << e.what() << std::endl << std::endl; \
  }

#define EOWU_PRINT_TEST_BEGIN(name) \
  std::cout << "-------- TEST: " << name << std::endl << std::endl;
