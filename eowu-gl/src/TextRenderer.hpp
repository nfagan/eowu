//
//  TextRenderer.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#pragma once

#include "Identifier.hpp"
#include "Font.hpp"
#include "TextModel.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace eowu {
  class TextRenderer;
  class Window;
  class Program;
  class Material;
}

class eowu::TextRenderer {
public:
  struct DrawableComponent {
    eowu::TextModel model;
    std::shared_ptr<eowu::FontFace> font;
  };
  
public:
  TextRenderer();
  ~TextRenderer() = default;
  
  void Draw(const eowu::TextRenderer::DrawableComponent &component,
            std::shared_ptr<eowu::Window> window,
            glm::mat4 projection_matrix);
  
  void Queue(const eowu::TextRenderer::DrawableComponent &component,
             std::shared_ptr<eowu::Window> window);
  
  void RegisterWindow(std::shared_ptr<eowu::Window> window);
  void RegisterWindows(const std::vector<std::shared_ptr<eowu::Window>> &windows);
  
  void ClearQueue();
  
  void NextFrame();
  
  const std::vector<DrawableComponent>& GetQueuedComponents(std::shared_ptr<eowu::Window> for_window);
  
private:
  std::unordered_map<eowu::Identifier, eowu::FontQuad> font_quads_by_window_id;
  std::unordered_map<eowu::Identifier, std::shared_ptr<eowu::Program>> programs_by_material_id;
  std::unordered_map<std::string, std::shared_ptr<eowu::Program>> programs_by_source;
  std::unordered_map<eowu::Identifier, std::vector<DrawableComponent>> queued_components;
  
  std::string single_char;
  
private:
  eowu::FontQuad& get_font_quad(const eowu::Identifier &for_window_id);
  void create_font_quad(const eowu::Identifier &for_window_id, bool check_if_exists = true);
  std::shared_ptr<eowu::Program> get_program(const std::shared_ptr<eowu::Material> &material);
  void require_models_container(const eowu::Identifier &id);
  void register_window(const std::shared_ptr<eowu::Window> &window);
};
