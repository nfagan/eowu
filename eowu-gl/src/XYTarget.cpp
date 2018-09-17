//
//  XYTarget.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/13/18.
//

#include "XYTarget.hpp"
#include "XYSource.hpp"
#include "Model.hpp"
#include "Units.hpp"
#include "Window.hpp"
#include <cmath>

#define EOWU_TARGET_CB_IMPL(function_name, member_name) \
  void eowu::XYTarget::function_name(const eowu::TargetCallbackType &func) { \
    std::unique_lock<std::mutex> lock(mut); \
    member_name = func; \
  }

namespace priv {
  bool bounds_noop(eowu::XYTarget *target, eowu::Coordinate coord) {
    return false;
  }
  
  void target_noop(eowu::XYTarget *target) {
    //
  }
}

void eowu::target_functions::reset(eowu::XYTarget *target) {
  target->Reset();
}

void eowu::target_functions::update_on_in_bounds(eowu::XYTarget *target) {
  target->IncrementTotalTimeInBounds();
}

bool eowu::bounds_functions::rect_in_bounds(eowu::XYTarget *target, eowu::Coordinate coord) {
  auto rect = target->GetTransform().GetBoundingRect();
  auto pad = target->GetUnitsPadding();
  
  double half_x = pad.x / 2.0;
  double half_y = pad.y / 2.0;
  
  double x0 = rect[0] - half_x;
  double x1 = rect[2] + half_x;
  double y0 = rect[1] - half_y;
  double y1 = rect[3] + half_y;
  
  double x = coord.x;
  double y = coord.y;
  
  return x >= x0 && x <= x1 && y >= y0 && y <= y1;
}

bool eowu::bounds_functions::circle_in_bounds(eowu::XYTarget *target, eowu::Coordinate coord) {
  const auto &transform = target->GetTransform();
  
  auto pos = transform.GetUnitsPosition();
  auto scl = transform.GetUnitsScale();
  auto pad = target->GetUnitsPadding();
  
  auto x_scale = (scl.x + pad.x) / 2.0;
  auto y_scale = (scl.y + pad.y) / 2.0;
  //  radius is greater of x-scale or y-scale
  auto radius = x_scale > y_scale ? x_scale : y_scale;
  
  auto x_delta = pos.x - coord.x;
  auto y_delta = pos.y - coord.y;
  
  auto x_delta2 = x_delta * x_delta;
  auto y_delta2 = y_delta * y_delta;
  
  auto d = std::sqrt(x_delta2 + y_delta2);
  
  return d <= radius;
}

eowu::XYTarget::XYTarget() : source(nullptr), window(nullptr), padding({0, 0}), linked_model(nullptr) {
  set_default_callbacks();
  Reset();
}

eowu::XYTarget::XYTarget(const eowu::XYTarget &other) : timer(other.GetTimer()) {
  std::unique_lock<std::mutex> lock(other.mut);
  
  source = other.source;
  window = other.window;
  transform = other.transform;
  padding = other.padding;
  
  linked_model = other.linked_model;
  
  entered = other.entered.load();
  exited = other.exited.load();
  
  on_entry = other.on_entry;
  on_exit = other.on_exit;
  
  on_ib = other.on_ib;
  on_oob = other.on_oob;
  
  bounds_function = other.bounds_function;
  
  total_time_in_bounds = other.total_time_in_bounds.load();
}

void eowu::XYTarget::LinkModel(std::shared_ptr<eowu::Model> model) {
  std::unique_lock<std::mutex> lock(mut);
  
  linked_model = model;
}

void eowu::XYTarget::Reset() {
  entered = false;
  exited = false;
  total_time_in_bounds = eowu::time::zero();
}

void eowu::XYTarget::Update() {
  std::unique_lock<std::mutex> lock(mut);
  
  if (source == nullptr || !source->IsValid()) {
    return;
  }
  
  if (has_linked_model()) {
    match_to_linked_model();
  }
  
  if (window != nullptr) {
    transform.SetScreenDimensions(window->GetDimensions());
  }
  
  auto coord = source->GetLatestSample();
  bool entered = Entered();
  bool exited = Exited();
  
  if (bounds_function(this, coord)) {
    if (!entered) {
      entry();
    } else if (exited) {
      //  @TODO: Handle reentry.
    }
    
    in_bounds();
  } else {
    if (entered) {
      exit();
    }
    
    out_of_bounds();
  }
}

void eowu::XYTarget::IncrementTotalTimeInBounds() {
  total_time_in_bounds = total_time_in_bounds.load() + timer.Ellapsed();
  timer.Reset();
}

bool eowu::XYTarget::Entered() const {
  return entered;
}

bool eowu::XYTarget::Exited() const {
  return exited;
}

EOWU_TARGET_CB_IMPL(SetOnEntry, on_entry);
EOWU_TARGET_CB_IMPL(SetOnExit, on_exit);
EOWU_TARGET_CB_IMPL(SetOnInBounds, on_ib);
EOWU_TARGET_CB_IMPL(SetOnOutOfBounds, on_oob);

void eowu::XYTarget::SetPadding(const glm::vec2 &padding_) {
  std::unique_lock<std::mutex> lock(mut);
  
  padding = padding_;
}

void eowu::XYTarget::SetSource(const eowu::XYSource *source_) {
  std::unique_lock<std::mutex> lock(mut);
  
  source = source_;
}

void eowu::XYTarget::SetWindow(const eowu::Window *window_) {
  std::unique_lock<std::mutex> lock(mut);
  
  window = window_;
}

void eowu::XYTarget::SetBoundsFunction(const eowu::BoundsCallbackType &func) {
  std::unique_lock<std::mutex> lock(mut);
  
  bounds_function = func;
}

eowu::Transform& eowu::XYTarget::GetTransform() {
  return transform;
}

const eowu::Transform& eowu::XYTarget::GetTransform() const {
  return transform;
}

eowu::Timer& eowu::XYTarget::GetTimer() {
  return timer;
}

const eowu::Timer& eowu::XYTarget::GetTimer() const {
  return timer;
}

const eowu::XYSource* eowu::XYTarget::GetSource() const {
  return source;
}

glm::vec2 eowu::XYTarget::GetUnitsPadding() const {
  //  @Robustness: Currently, changes to padding in multiple
  //  threads is unsafe. Consider changing std::mutex to
  //  std::recursive_mutex to allow GetUnitsPadding() to be called
  //  from within a bounds function.
  
  auto units = transform.GetUnits();
  auto screen_dims = transform.GetScreenDimensions();
  auto pixel_padding = padding;
  
  if (units == eowu::units::normalized || units == eowu::units::mixed) {
    pixel_padding.x = pixel_padding.x * screen_dims.x;
    pixel_padding.y = pixel_padding.y * screen_dims.y;
  }
  
  return pixel_padding;
}

eowu::time::DurationType eowu::XYTarget::GetTotalTimeInBounds() const {
  return total_time_in_bounds.load();
}

void eowu::XYTarget::in_bounds() {
  on_ib(this);
}

void eowu::XYTarget::out_of_bounds() {
  on_oob(this);
}

void eowu::XYTarget::entry() {
  std::cout << "Entered! " << std::endl;
  
  entered = true;
  on_entry(this);
}

void eowu::XYTarget::exit() {
  std::cout << "Exited! " << std::endl;
  
  on_exit(this);
  entered = false;
  exited = true;
}

void eowu::XYTarget::set_default_callbacks() {
  bounds_function = priv::bounds_noop;
  on_entry = priv::target_noop;
  on_exit = priv::target_noop;
  on_ib = priv::target_noop;
  on_oob = priv::target_noop;
}

void eowu::XYTarget::match_to_linked_model() {
  const auto &model_transform = linked_model->GetTransform();
  
  transform.SetUnits(model_transform.GetUnits());
  transform.SetPosition(model_transform.GetPosition());
  transform.SetScale(model_transform.GetScale());
}

bool eowu::XYTarget::has_linked_model() const {
  return linked_model != nullptr;
}


//
//  Make
//

eowu::XYTarget eowu::XYTarget::MakeCommon() {
  eowu::XYTarget target;
  
  target.SetOnEntry(eowu::target_functions::reset);
  target.SetOnInBounds(eowu::target_functions::update_on_in_bounds);
  target.SetBoundsFunction(eowu::bounds_functions::circle_in_bounds);
  
  return target;
}
