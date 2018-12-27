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

eowu::Transform::Transform() :
units(eowu::units::pixels),
position(glm::vec3(0.0f)),
rotation(glm::vec3(0.0f)),
scale(glm::vec3(0.0f)),
screen_dimensions(glm::vec2(0.0f)) {
  //
}

eowu::Transform::Transform(const eowu::Transform &other) :
units(other.GetUnits()),
position(other.GetPosition()),
rotation(other.GetRotation()),
scale(other.GetScale()),
screen_dimensions(other.GetScreenDimensions()) {
  //
}

eowu::Transform::Transform(eowu::Transform &&other) noexcept :
units(std::move(other.GetUnits())),
position(std::move(other.GetPosition())),
rotation(std::move(other.GetRotation())),
scale(std::move(other.GetScale())),
screen_dimensions(std::move(other.GetScreenDimensions())) {
  //
}

eowu::Transform& eowu::Transform::operator=(eowu::Transform &&other) noexcept {
  units = std::move(GetUnits());
  
  position = std::move(other.GetPosition());
  rotation = std::move(other.GetRotation());
  scale = std::move(other.GetScale());
  screen_dimensions = std::move(other.GetScreenDimensions());
  
  return *this;
}

eowu::Transform& eowu::Transform::operator=(const eowu::Transform &other) {
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

unsigned int eowu::Transform::GetUnits() const {
  return units;
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

glm::vec3 eowu::Transform::GetUnitsPosition() const {
  auto pos = GetPosition();
  auto dims = GetScreenDimensions();
  
  auto un = units.load();
  
  if (un == eowu::units::normalized || un == eowu::units::mixed) {
    pos.x *= dims.x;
    pos.y *= dims.y;
  }
  
  return pos;
}

glm::vec3 eowu::Transform::GetYInvertedUnitsPosition() const {
  auto pos = GetPosition();
  auto dims = GetScreenDimensions();
  auto un = units.load();
  
  if (un == eowu::units::normalized || un == eowu::units::mixed) {
    pos.y *= dims.y;
    pos.x *= dims.x;
  }
  
  pos.y = dims.y - pos.y;
  
  return pos;
}

glm::vec3 eowu::Transform::GetUnitsScale() const {
  auto scl = GetScale();
  auto dims = GetScreenDimensions();
  
  if (units.load() == eowu::units::normalized) {
    scl.x *= dims.x;
    scl.y *= dims.y;
  }
  
  return scl;
}

glm::vec4 eowu::Transform::GetBoundingRect(bool inverted_y) const {
  glm::vec4 rect;
  glm::vec3 pos;
  
  if (inverted_y) {
    pos = GetYInvertedUnitsPosition();
  } else {
    pos = GetPosition();
  }
  
  auto scl = GetUnitsScale();
  
  float half_width = scl.x / 2.0;
  float half_height = scl.y / 2.0;
  
  rect[0] = pos.x - half_width;
  rect[1] = pos.y - half_height;
  rect[2] = pos.x + half_width;
  rect[3] = pos.y + half_height;
  
  return rect;
}

glm::mat4 eowu::Transform::GetTransformationMatrix() const {
  const auto rot = GetRotation();
  const auto pos = GetUnitsPosition();
  const auto scl = GetUnitsScale();
  
  glm::mat4 transform(1.0f);
  
  transform = glm::translate(transform, pos);
  transform = glm::rotate(transform, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
  
  return glm::scale(transform, scl);
}

void eowu::Transform::MakeLike(const eowu::Transform &other) {
  SetPosition(other.GetPosition());
  SetRotation(other.GetRotation());
  SetScale(other.GetScale());
  SetUnits(other.GetUnits());
  SetScreenDimensions(other.GetScreenDimensions());
}
