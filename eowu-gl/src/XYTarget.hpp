//
//  XYTarget.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#pragma once

#include "XYSource.hpp"
#include "Transform.hpp"
#include <eowu-common/time.hpp>
#include <eowu-common/Timer.hpp>
#include <glm/glm.hpp>
#include <functional>
#include <mutex>
#include <atomic>
#include <memory>

namespace eowu {
  class XYTarget;
  class Model;
  class Window;
  
  using TargetCallbackType = std::function<void(eowu::XYTarget*)>;
  using BoundsCallbackType = std::function<bool(eowu::XYTarget*, eowu::Coordinate)>;
  
  namespace target_functions {
    void reset(eowu::XYTarget* target);
    void update_on_in_bounds(eowu::XYTarget* target);
  }
  
  namespace bounds_functions {
    bool circle_in_bounds(eowu::XYTarget *target, eowu::Coordinate coord);
    bool rect_in_bounds(eowu::XYTarget *target, eowu::Coordinate coord);
  }
}

class eowu::XYTarget {
public:
  XYTarget();
  XYTarget(const eowu::XYTarget &other);
  ~XYTarget() = default;
  
  void Reset();
  void Update();
  void IncrementTotalTimeInBounds();
  
  void SetPadding(const glm::vec2 &padding);
  void SetSource(const eowu::XYSource *source);
  void SetWindow(const eowu::Window *window);
  void SetBoundsFunction(const eowu::BoundsCallbackType &func);
  void SetOnEntry(const eowu::TargetCallbackType &func);
  void SetOnExit(const eowu::TargetCallbackType &func);
  void SetOnInBounds(const eowu::TargetCallbackType &func);
  void SetOnOutOfBounds(const eowu::TargetCallbackType &func);
  
  void LinkModel(std::shared_ptr<eowu::Model> model);
  
  bool Entered() const;
  bool Exited() const;
  
  eowu::Transform& GetTransform();
  const eowu::Transform& GetTransform() const;
  eowu::Timer& GetTimer();
  const eowu::Timer& GetTimer() const;
  eowu::time::DurationType GetTotalTimeInBounds() const;
  const eowu::XYSource* GetSource() const;
  glm::vec2 GetUnitsPadding() const;
  
private:
  mutable std::mutex mut;
  
  const eowu::XYSource *source;
  const eowu::Window *window;
  
  eowu::Transform transform;
  glm::vec2 padding;
  
  std::shared_ptr<eowu::Model> linked_model;
  
  std::atomic<bool> entered;
  std::atomic<bool> exited;
  
  eowu::TargetCallbackType on_entry;
  eowu::TargetCallbackType on_exit;
  eowu::TargetCallbackType on_ib;
  eowu::TargetCallbackType on_oob;
  
  eowu::BoundsCallbackType bounds_function;
  
  eowu::Timer timer;
  std::atomic<eowu::time::DurationType> total_time_in_bounds;
  
  void set_default_callbacks();
  void entry();
  void exit();
  void in_bounds();
  void out_of_bounds();
  
  void match_to_linked_model();
  bool has_linked_model() const;
};
