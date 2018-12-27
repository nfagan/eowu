//
//  TextModel.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#pragma once

#include "Transform.hpp"
#include "Identifier.hpp"
#include <string>
#include <functional>
#include <atomic>

namespace eowu {
  class TextModel;
  class Material;
  class FontFace;
}

class eowu::TextModel {
public:
  TextModel();
  ~TextModel() = default;
  
  TextModel(const eowu::TextModel &other);
  
  void SetText(const std::string &text);
  void SetMaterial(std::shared_ptr<eowu::Material> material);
  void SetScale(double scale);
  void SetIsCentered(bool is_centered);
  
  eowu::Transform& GetTransform();
  const eowu::Transform& GetTransform() const;
  std::shared_ptr<eowu::Material> GetMaterial() const;
  std::string GetText() const;
  glm::vec3 CalculateCenterOffset(std::shared_ptr<eowu::FontFace> for_font_face) const;
  double GetScale() const;
  
  bool IsCentered() const;
  
  void UseText(const std::function<void(const std::string&)> &callback) const;
  void NextFrame();
  
private:
  mutable std::mutex mut;
  
  eowu::Identifier resource_id;
  
  std::string text;
  
  std::shared_ptr<eowu::Material> material;
  eowu::Transform transform;
  
  std::atomic<bool> is_centered;
};
