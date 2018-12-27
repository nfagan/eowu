//
//  State.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "State.hpp"
#include "ExitConditions.hpp"
#include "StateManager.hpp"

namespace detail {
  void no_op(eowu::State *state) {}
}

eowu::State::State(const eowu::StateManager *manager_, const std::string &id_, const eowu::Timer *global_timer_) :
manager(manager_), global_timer(global_timer_), id(id_), next_state(nullptr), called_next(false) {
  set_default_callbacks();
}

eowu::State::State(const eowu::State &other) {
  std::unique_lock<std::mutex> lock(other.mut);
  
  SetOnEntry(other.on_entry);
  SetOnExit(other.on_exit);
  SetOnLoop(other.on_loop);
  
  id = other.id;
  manager = other.manager;
  global_timer = other.global_timer;
  
  called_next = false;
  next_state = nullptr;
}

void eowu::State::OnEntry() {
  std::unique_lock<std::mutex> lock(mut);
  //  call private entry handler first
  entry();
  on_entry(this);
}

void eowu::State::OnExit() {
  std::unique_lock<std::mutex> lock(mut);
  //  call private exit handler first
  exit();
  on_exit(this);
}

void eowu::State::OnLoop() {
  std::unique_lock<std::mutex> lock(mut);
  on_loop(this);
}

void eowu::State::AddExitCondition(const eowu::ExitConditionType &condition) {
  persistent_exit_conditions.push_back(condition);
}

void eowu::State::SetDuration(eowu::time::DurationType duration) {
  auto condition = std::bind(&eowu::exit_conditions::time_exceeded, std::placeholders::_1, duration);
  AddExitCondition(condition);
}

void eowu::State::SetOnEntry(const eowu::StateCallbackType &cb) {
  std::unique_lock<std::mutex> lock(mut);
  on_entry = cb;
}

void eowu::State::SetOnExit(const eowu::StateCallbackType &cb) {
  std::unique_lock<std::mutex> lock(mut);
  on_exit = cb;
}

void eowu::State::SetGlobalTimer(const eowu::Timer *global_timer) {
  std::unique_lock<std::mutex> lock(mut);
  this->global_timer = global_timer;
}

void eowu::State::Next(eowu::State *state) {
  next_state = state;
}

void eowu::State::Exit(eowu::State *state) {
  next_state = state;
  called_next = true;
}

void eowu::State::Exit() {
//  Exit(nullptr);
  called_next = true;
}

eowu::State* eowu::State::GetNext() {
  return next_state;
}

const std::string& eowu::State::GetId() const {
  return id;
}

eowu::State* eowu::State::GetState(const std::string &id) const {
  return manager->GetState(id);
}

const eowu::Timer& eowu::State::GetTimer() const {
  return local_timer;
}

eowu::State::GlobalTimePoints eowu::State::GetLatestGlobalTimePoints() const {
  return global_time_points;
}

void eowu::State::SetOnLoop(const eowu::StateCallbackType &cb) {
  std::unique_lock<std::mutex> lock(mut);
  on_loop = cb;
}

bool eowu::State::ShouldExit() const {
  return called_next || check_exit_conditions();
}

void eowu::State::entry() {
  local_timer.Reset();
  Next(nullptr);
  called_next = false;
  
  //  Mark the time of entry in terms of the global clock.
  if (global_timer) {
    global_time_points.entry = global_timer->Elapsed();
  }
}

void eowu::State::exit() {
  //  Mark the time of exit in terms of the global clock.
  if (global_timer) {
    global_time_points.exit = global_timer->Elapsed();
  }
}

bool eowu::State::check_exit_conditions() const {
  for (const auto &cnd : persistent_exit_conditions) {
    if (cnd(this)) {
      return true;
    }
  }
  
  return false;
}

void eowu::State::set_default_callbacks() {
  SetOnEntry(&detail::no_op);
  SetOnExit(&detail::no_op);
  SetOnLoop(&detail::no_op);
}
