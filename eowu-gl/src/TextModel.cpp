//
//  TextModel.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#include "TextModel.hpp"
#include "Material.hpp"
#include "Font.hpp"

eowu::TextModel::TextModel() : is_centered(false) {
  SetScale(1.0);
}

eowu::TextModel::TextModel(const eowu::TextModel &other) :
text(other.GetText()),
material(other.GetMaterial()),
transform(other.GetTransform()),
is_centered(other.IsCentered()) {
  //
}

void eowu::TextModel::SetText(const std::string &text) {
  std::lock_guard<std::mutex> lock(mut);
  this->text = text;
}

void eowu::TextModel::SetIsCentered(bool is_centered) {
  this->is_centered = is_centered;
}

void eowu::TextModel::SetMaterial(std::shared_ptr<eowu::Material> material) {
  std::lock_guard<std::mutex> lock(mut);
  this->material = material;
}

void eowu::TextModel::SetScale(double scale) {
  GetTransform().SetScale(glm::vec3(scale));
}

const eowu::Transform& eowu::TextModel::GetTransform() const {
  return transform;
}

eowu::Transform& eowu::TextModel::GetTransform() {
  return transform;
}

double eowu::TextModel::GetScale() const {
  return transform.GetScale().x;
}

std::shared_ptr<eowu::Material> eowu::TextModel::GetMaterial() const {
  std::lock_guard<std::mutex> lock(mut);
  return material;
}

std::string eowu::TextModel::GetText() const {
  std::lock_guard<std::mutex> lock(mut);
  return text;
}

bool eowu::TextModel::IsCentered() const {
  return is_centered;
}

void eowu::TextModel::UseText(const std::function<void (const std::string &)> &callback) const {
  std::lock_guard<std::mutex> lock(mut);
  callback(text);
}

void eowu::TextModel::NextFrame() {
  std::lock_guard<std::mutex> lock(mut);
  
  if (material) {
    material->NextFrame();
  }
}

glm::vec3 eowu::TextModel::CalculateCenterOffset(std::shared_ptr<eowu::FontFace> for_font_face) const {
  //  @TODO: Vertical centering as well.
  std::lock_guard<std::mutex> lock(mut);
  
  std::size_t n_characters = text.size();
  glm::vec3 center_offset(0.0f);
  
  if (n_characters == 0) {
    return center_offset;
  }
  
  float scale = float(GetScale());
  float total_width = 0.0f;
  
  std::size_t idx = 1;
  
  for (const auto &character : text) {
    if (!for_font_face->HasGlyph(character)) {
      continue;
    }
    
    const auto &glyph = for_font_face->GetGlyph(character);
    
    const auto &size = glyph.GetSize();
//    const auto &bearing = glyph.GetBearing();
    const auto advance = glyph.GetAdvance();
    
    float next_x = float(advance >> 6) * scale;
    
    total_width += next_x;
    
    if (idx == n_characters) {
      total_width += (size.x * scale);
    }
  }
  
  center_offset.x = -total_width / 2.0f;
  
  return center_offset;
}
