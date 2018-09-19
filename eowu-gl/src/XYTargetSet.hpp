//
//  TargetSet.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#pragma once

#include <eowu-common/time.hpp>
#include <vector>
#include <functional>
#include <mutex>
#include <atomic>

namespace eowu {
  class XYTarget;
  class XYTargetSet;
}

class eowu::XYTargetSet {
public:
  XYTargetSet();
  XYTargetSet(const std::vector<eowu::XYTarget*> &targets);
  ~XYTargetSet();
  
  void SetTargets(const std::vector<eowu::XYTarget*> &targets);
  void SetOnEntry(const std::function<void(eowu::XYTarget*)> &cb);
  void SetOnExit(const std::function<void(eowu::XYTarget*)> &cb);
  void SetOnSelect(const std::function<void(eowu::XYTarget*)> &cb);
  void SetDuration(const eowu::time::DurationType &dur);
  void Reset();
  void Begin();
  
  bool MadeSelection() const;
  bool CalledSelected() const;
  
private:
  mutable std::recursive_mutex mut;
  
  std::vector<eowu::XYTarget*> targets;
  eowu::XYTarget *selected_target;
  
  std::atomic<eowu::time::DurationType> duration;
  std::atomic<bool> called_selected;
  
  std::function<void(eowu::XYTarget*)> on_entry;
  std::function<void(eowu::XYTarget*)> on_exit;
  std::function<void(eowu::XYTarget*)> on_select;
  
  void configure_targets();
  void reset_targets();
  void release_targets();
  void assign_default_callbacks();
  
  void in_bounds_handler(eowu::XYTarget *targ);
  void entry_handler(eowu::XYTarget *targ);
  void exit_handler(eowu::XYTarget *targ);
  void selection_handler(eowu::XYTarget *targ);
  
  static void target_noop(eowu::XYTarget *targ);
};
