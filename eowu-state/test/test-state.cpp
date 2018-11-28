//
//  test-state.cpp
//  eowu-state
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-state.hpp"
#include <iostream>
#include <assert.h>
#include <cmath>

void test_multiple_state() {
  eowu::StateManager manager;
  eowu::StateRunner runner;
  
  auto state1 = manager.CreateState("1");
  auto state2 = manager.CreateState("2");
  
  bool entered2 = false;
  
  state2->SetOnEntry([&entered2](auto st) {
    entered2 = true;
  });
  
  state1->SetOnEntry([](auto st) {
    st->Next(st->GetState("2"));
  });
  
  state1->SetDuration(std::chrono::milliseconds(1000));
  state2->SetDuration(std::chrono::seconds(1));
  
  runner.Begin(state1);
  
  bool should_quit = false;
  while (!should_quit) {
    should_quit = runner.Update();
  }
  
  if (!entered2) {
    std::cout << "ERROR: Never entered state2" << std::endl;
  } else {
    std::cout << "OK: Entered state2" << std::endl;
  }
}

void test_exit_condition() {
  eowu::StateManager manager;
  eowu::StateRunner runner;
  
  auto res = manager.CreateState("id");
  double thresh = 1e-4;
  auto amt = 1000;
  auto dur = std::chrono::milliseconds(amt);
  
  res->SetDuration(dur);
  res->SetOnEntry([](auto state) {
    std::cout << "Entered" << std::endl;
  });
  
  res->SetOnExit([](auto state) {
//    state->Next(state);
  });
  
  bool should_quit = false;
  
  runner.Begin(res);
  
  auto t1 = eowu::time::now();
  
  while (!should_quit) {
    should_quit = runner.Update();
  }
  
  auto t2 = eowu::time::now();
  
  auto ellapsed = std::chrono::duration<double>(t2 - t1);
  auto time_ellapsed = res->GetTimer().Elapsed();
  
  double difference = time_ellapsed.count() - ellapsed.count();
  
  if (abs(difference) > thresh) {
    std::cout << "ERROR: Failed to run state for " << amt << " seconds; difference was: " << difference << std::endl;
  } else {
    std::cout << "OK: Ran state for " << amt << " seconds; difference was: " << difference << std::endl;
  }
}

void test_create_duplicates() {
  eowu::StateManager manager;
  
  auto res = manager.CreateState("id");
  
  try {
    auto res2 = manager.CreateState("id");
    std::cout << "ERROR: Allowed duplicate state" << std::endl;
  } catch (const std::exception &e) {
    std::cout << "OK: Attempting to create duplicate state failed with message: " << e.what() << std::endl;
  }
  
  assert(manager.HasState("id"));
}

void test_state_run_all() {
  std::cout << "--------" << "state" << std::endl;
  test_create_duplicates();
  test_exit_condition();
  test_multiple_state();
}
