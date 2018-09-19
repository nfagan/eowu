//
//  TargetSet.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#include "XYTargetSet.hpp"
#include "XYTarget.hpp"
#include <functional>
#include <assert.h>

#define EOWU_TARGET_SET_CB_IMPL(name, member) \
  void eowu::XYTargetSet::name(const std::function<void(eowu::XYTarget*)> &cb) { \
    std::unique_lock<std::recursive_mutex> lock(mut); \
    member = cb; \
  }

eowu::XYTargetSet::XYTargetSet() :
selected_target(nullptr), duration(eowu::time::zero()), called_selected(false) {
  assign_default_callbacks();
}

eowu::XYTargetSet::XYTargetSet(const std::vector<eowu::XYTarget*> &targets_) :
targets(targets_), selected_target(nullptr), duration(eowu::time::zero()), called_selected(false) {
  assign_default_callbacks();
}

eowu::XYTargetSet::~XYTargetSet() {
  Reset();
}

void eowu::XYTargetSet::Begin() {
  configure_targets();
}

void eowu::XYTargetSet::Reset() {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  assign_default_callbacks();
  reset_targets();
  release_targets();
  
  selected_target = nullptr;
  called_selected = false;
}

EOWU_TARGET_SET_CB_IMPL(SetOnEntry, on_entry);
EOWU_TARGET_SET_CB_IMPL(SetOnExit, on_exit);
EOWU_TARGET_SET_CB_IMPL(SetOnSelect, on_select);

bool eowu::XYTargetSet::MadeSelection() const {
  return selected_target != nullptr;
}

bool eowu::XYTargetSet::CalledSelected() const {
  return called_selected;
}

void eowu::XYTargetSet::SetTargets(const std::vector<eowu::XYTarget*> &targets_) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  targets = targets_;
}

void eowu::XYTargetSet::SetDuration(const eowu::time::DurationType &dur) {
  duration = dur;
}

void eowu::XYTargetSet::assign_default_callbacks() {
  SetOnEntry(&eowu::XYTargetSet::target_noop);
  SetOnExit(&eowu::XYTargetSet::target_noop);
  SetOnSelect(&eowu::XYTargetSet::target_noop);
}

void eowu::XYTargetSet::configure_targets() {
  using namespace std::placeholders;
  
  auto entry_callback = std::bind(&eowu::XYTargetSet::entry_handler, this, _1);
  auto exit_callback = std::bind(&eowu::XYTargetSet::exit_handler, this, _1);
  auto ib_callback = std::bind(&eowu::XYTargetSet::in_bounds_handler, this, _1);
  
  for (auto &targ : targets) {
    assert(targ != nullptr);
    
    //  mark that the target is part of a set now.
    targ->set_part_of_set(true);
    targ->SetOnEntry(entry_callback);
    targ->SetOnExit(exit_callback);
    targ->SetOnInBounds(ib_callback);
    targ->SetOnOutOfBounds([](auto *targ) -> void {});
    targ->Reset();
  }
}

void eowu::XYTargetSet::reset_targets() {
  for (auto &targ : targets) {
    targ->Reset();
  }
}

void eowu::XYTargetSet::release_targets() {
  for (auto &targ : targets) {
    targ->set_part_of_set(false);
  }
}

void eowu::XYTargetSet::in_bounds_handler(eowu::XYTarget *targ) {  
  if (targ != selected_target) {
    return;
  }
  
  auto ellapsed = targ->GetTotalTimeInBounds();
  
  if (ellapsed >= duration.load() && !CalledSelected()) {
    selection_handler(targ);
  }
}

void eowu::XYTargetSet::selection_handler(eowu::XYTarget *targ) {
  on_select(targ);
  called_selected = true;
}

void eowu::XYTargetSet::entry_handler(eowu::XYTarget *targ) {
  if (!MadeSelection()) {
    selected_target = targ;
    on_entry(targ);
  }
}

void eowu::XYTargetSet::exit_handler(eowu::XYTarget *targ) {
  if (targ == selected_target) {
    on_exit(targ);
    selected_target = nullptr;
    called_selected = false;
  }
  
  reset_targets();
}

void eowu::XYTargetSet::target_noop(eowu::XYTarget *target) {
  //
}
