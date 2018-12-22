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

double eowu::padding::get_radius(const glm::vec3 &scale, const glm::vec2 &padding) {
  auto x_scale = (scale.x + padding.x) / 2.0;
  auto y_scale = (scale.y + padding.y) / 2.0;
  //  radius is greater of x-scale or y-scale
  double radius = x_scale > y_scale ? x_scale : y_scale;
  
  return radius;
}

glm::vec3 eowu::padding::get_circ_scale(const glm::vec3 &scale, const glm::vec2 &padding) {
  double diam = get_radius(scale, padding) * 2.0;
  
  return glm::vec3(diam, diam, scale.z);
}

glm::vec3 eowu::padding::get_rect_scale(const glm::vec3 &scale, const glm::vec2 &padding) {
  glm::vec3 result(scale);
  
  result.x += padding.x;
  result.y += padding.y;
  
  return result;
}

void eowu::target_functions::reset(eowu::XYTarget *target) {
  target->Reset();
}

bool eowu::bounds_functions::rect_in_bounds(eowu::XYTarget *target, eowu::Coordinate coord) {
  bool is_inverted = true;
  
  auto rect = target->GetTransform().GetBoundingRect(is_inverted);
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
  
  auto pos = transform.GetYInvertedUnitsPosition();
  auto scl = transform.GetUnitsScale();
  auto pad = target->GetUnitsPadding();
  
  double x = coord.x;
  double y = coord.y;
  
  double h = pos.x;
  double k = pos.y;
  
  double rx = (scl.x + pad.x) / 2.0;
  double ry = (scl.y + pad.y) / 2.0;
  
  double rx2 = rx * rx;
  double ry2 = ry * ry;
  
  double xd = x - h;
  double yd = y - k;
  
  double xd2 = xd * xd;
  double yd2 = yd * yd;
  
  double x_component = xd2 / rx2;
  double y_component = yd2 / ry2;
  
  return x_component + y_component <= 1;
}

eowu::XYTarget::XYTarget() :
source(nullptr), window(nullptr), padding({0, 0}), linked_model(nullptr), is_part_of_set(false),
entered(false), exited(false), is_in_bounds(false) {
  set_default_callbacks();
  Reset();
}

eowu::XYTarget::XYTarget(const eowu::XYTarget &other) : timer(other.timer) {
  std::unique_lock<std::mutex> lock(other.mut);
  
  source = other.source;
  window = other.window;
  transform = other.transform;
  padding = other.padding;
  
  linked_model = other.linked_model;
  
  entered = other.entered.load();
  exited = other.exited.load();
  is_in_bounds = other.is_in_bounds.load();
  
  on_entry = other.on_entry;
  on_exit = other.on_exit;
  
  is_part_of_set = other.is_part_of_set.load();
  
  on_ib = other.on_ib;
  on_oob = other.on_oob;
  
  bounds_function = other.bounds_function;
  
  total_time_in_bounds = other.total_time_in_bounds.load();
  
  alias = other.alias;
}

void eowu::XYTarget::LinkModel(std::shared_ptr<eowu::Model> model) {
  std::unique_lock<std::mutex> lock(mut);
  
  linked_model = model;
}

void eowu::XYTarget::Reset() {
  entered = false;
  exited = false;
  total_time_in_bounds = eowu::time::zero();
  timer.Reset();
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
  
  if (bounds_function(this, coord)) {
    if (!entered) {
      entry();
    }
    
    in_bounds();
  } else {
    if (entered) {
      exit();
    }
    
    out_of_bounds();
  }
}

bool eowu::XYTarget::Entered() const {
  return entered;
}

bool eowu::XYTarget::Exited() const {
  return exited;
}

bool eowu::XYTarget::IsInBounds() const {
  return is_in_bounds;
}

bool eowu::XYTarget::IsPartOfSet() const {
  return is_part_of_set;
}

EOWU_TARGET_CB_IMPL(SetOnEntry, on_entry);
EOWU_TARGET_CB_IMPL(SetOnExit, on_exit);
EOWU_TARGET_CB_IMPL(SetOnInBounds, on_ib);
EOWU_TARGET_CB_IMPL(SetOnOutOfBounds, on_oob);

void eowu::XYTarget::SetAlias(const std::string &alias_) {
  std::unique_lock<std::mutex> lock(mut);
  
  alias = alias_;
}

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

const eowu::XYSource* eowu::XYTarget::GetSource() const {
  return source;
}

std::string eowu::XYTarget::GetAlias() const {
  return alias;
}

glm::vec2 eowu::XYTarget::GetUnitsPadding() const {
  //  @Robustness: Currently, changes to padding in multiple
  //  threads is unsafe. Consider changing std::mutex to
  //  std::recursive_mutex to allow GetUnitsPadding() to be called
  //  from within a bounds function.
  
  auto units = transform.GetUnits();
  auto screen_dims = transform.GetScreenDimensions();
  auto pixel_padding = padding;
  
  if (units == eowu::units::normalized) {
    pixel_padding.x = pixel_padding.x * screen_dims.x;
    pixel_padding.y = pixel_padding.y * screen_dims.y;
  }
  
  return pixel_padding;
}

eowu::time::DurationType eowu::XYTarget::GetTotalTimeInBounds() const {
  return total_time_in_bounds.load();
}

void eowu::XYTarget::in_bounds() {
  is_in_bounds = true;
  total_time_in_bounds = timer.Elapsed();
  on_ib(this);
}

void eowu::XYTarget::out_of_bounds() {
  is_in_bounds = false;
  on_oob(this);
}

void eowu::XYTarget::entry() {
  entered = true;
  reset_time();
  on_entry(this);
}

void eowu::XYTarget::exit() {
  on_exit(this);
  entered = false;
  exited = true;
  reset_time();
}

void eowu::XYTarget::set_default_callbacks() {
  bounds_function = priv::bounds_noop;
  on_entry = priv::target_noop;
  on_exit = priv::target_noop;
  on_ib = priv::target_noop;
  on_oob = priv::target_noop;
}

void eowu::XYTarget::set_part_of_set(bool is_part_of_set_) {
  is_part_of_set = is_part_of_set_;
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

void eowu::XYTarget::reset_time() {
  timer.Reset();
  total_time_in_bounds = eowu::time::zero();
}
