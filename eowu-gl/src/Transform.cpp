//
//  Transform.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Transform.hpp"
#include "Units.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <mutex>
#include <utility>
#include "Error.hpp"

eowu::Transform::Transform() {
  position = glm::vec3(0.0f);
  rotation = glm::vec3(0.0f);
  scale = glm::vec3(0.0f);
  screen_dimensions = glm::vec2(0.0f);
  
  units = eowu::units::pixels;
}

eowu::Transform::Transform(const eowu::Transform &other) {
  position = other.GetPosition();
  rotation = other.GetRotation();
  scale = other.GetScale();
  screen_dimensions = other.GetScreenDimensions();
  
  units = other.units.load();
}

eowu::Transform::Transform(eowu::Transform &&other) noexcept {
  position = std::move(other.GetPosition());
  rotation = std::move(other.GetRotation());
  scale = std::move(other.GetScale());
  screen_dimensions = std::move(other.GetScreenDimensions());
  
  units = std::move(other.units.load());
}

eowu::Transform& eowu::Transform::operator=(eowu::Transform&& other) noexcept {
  position = std::move(other.GetPosition());
  rotation = std::move(other.GetRotation());
  scale = std::move(other.GetScale());
  screen_dimensions = std::move(other.GetScreenDimensions());
  
  units = std::move(other.units.load());
  
  return *this;
}

eowu::Transform& eowu::Transform::operator=(const eowu::Transform& other) {
  eowu::Transform tmp(other);
  *this = std::move(tmp);
  return *this;
}

void eowu::Transform::SetPosition(const glm::vec3 &pos) {
  std::unique_lock<std::shared_mutex> lock(mut);
  position = pos;
}

void eowu::Transform::SetScale(const glm::vec3 &scl) {
  std::unique_lock<std::shared_mutex> lock(mut);
  scale = scl;
}

void eowu::Transform::SetUnits(unsigned int units) {
  this->units.store(units);
}

void eowu::Transform::SetRotation(const glm::vec3 &rot) {
  std::unique_lock<std::shared_mutex> lock(mut);
  rotation = rot;
}

void eowu::Transform::SetScreenDimensions(const glm::vec2 &dims) {
  std::unique_lock<std::shared_mutex> lock(mut);
  screen_dimensions = dims;
}

glm::vec3 eowu::Transform::GetPosition() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return position;
}

glm::vec3 eowu::Transform::GetScale() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return scale;
}

glm::vec3 eowu::Transform::GetRotation() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return rotation;
}

glm::vec2 eowu::Transform::GetScreenDimensions() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return screen_dimensions;
}

glm::vec3 eowu::Transform::get_units_position() const {
  auto pos = GetPosition();
  
  if (units.load() == eowu::units::normalized) {
    pos.x *= screen_dimensions.x;
    pos.y *= screen_dimensions.y;
  }
  
  return pos;
}

glm::vec3 eowu::Transform::get_units_scale() const {
  auto scl = GetScale();
  
  if (units.load() == eowu::units::normalized) {
    scl.x *= screen_dimensions.x;
    scl.y *= screen_dimensions.y;
  }
  
  return scl;
}

glm::mat4 eowu::Transform::GetTransformationMatrix() const {
  const auto rot = GetRotation();
  const auto pos = get_units_position();
  const auto scl = get_units_scale();
  
  glm::mat4 transform(1.0f);
  
  transform = glm::translate(transform, pos);
  transform = glm::rotate(transform, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
  
  return glm::scale(transform, scl);
}

unsigned int eowu::Transform::GetUnitsFromStringLabel(const std::string &unit_id) {
  
  if (unit_id == "normalized") {
    return eowu::units::normalized;
  } else if (unit_id == "pixels") {
    return eowu::units::pixels;
  } else {
    throw eowu::UnrecognizedLabelError("No units matched '" + unit_id + "'");
  }  
}


