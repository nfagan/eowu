//
//  Transform.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <glm/glm.hpp>
#include <shared_mutex>
#include <atomic>
#include <string>
#include <vector>

namespace eowu {
  class Transform;
}

class eowu::Transform {
public:
  Transform();
  Transform(const eowu::Transform &other);
  Transform(eowu::Transform &&other) noexcept;
  
  Transform& operator=(const eowu::Transform& other);
  Transform& operator=(Transform&& other) noexcept;
  
  ~Transform() = default;
  
  void SetPosition(const glm::vec3 &pos);
  void SetRotation(const glm::vec3 &rot);
  void SetScale(const glm::vec3 &scale);
  void SetUnits(unsigned int units);
  void SetScreenDimensions(const glm::vec2 &dims);
  
  unsigned int GetUnits() const;
  glm::vec3 GetPosition() const;
  glm::vec3 GetRotation() const;
  glm::vec3 GetScale() const;
  glm::vec2 GetScreenDimensions() const;
  glm::vec3 GetUnitsPosition() const;
  glm::vec3 GetYInvertedUnitsPosition() const;
  glm::vec3 GetUnitsScale() const;
  glm::vec4 GetBoundingRect(bool inverted_y = false) const;
  
  glm::mat4 GetTransformationMatrix() const;
private:
  mutable std::shared_mutex mut;
  
  std::atomic<unsigned int> units;
  
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  
  glm::vec2 screen_dimensions;
};
