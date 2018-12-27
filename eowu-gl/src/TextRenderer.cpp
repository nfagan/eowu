//
//  TextRenderer.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/24/18.
//

#include "TextRenderer.hpp"
#include "Window.hpp"
#include "Program.hpp"
#include "Material.hpp"
#include "ShaderBuilder.hpp"
#include "UniformNames.hpp"
#include <glad/glad.h>
#include <eowu-common/logging.hpp>

eowu::TextRenderer::TextRenderer() :
single_char(std::string(1, 'c')) {
  //
}

void eowu::TextRenderer::Draw(const eowu::TextRenderer::DrawableComponent &component,
                              std::shared_ptr<eowu::Window> window,
                              glm::mat4 projection_matrix) {
  
  const auto &model = component.model;
  const auto &font = component.font;
  auto material = model.GetMaterial();
  
  if (material == nullptr) {
    return;
  }
  
  const auto &window_id = window->GetIdentifier();
  
  eowu::Transform transform(model.GetTransform());
  transform.SetScreenDimensions(window->GetDimensions());
  
  auto position = transform.GetUnitsPosition();
  //  Units are not accounted for in scale.
  auto scale = model.GetScale();
  
  if (model.IsCentered()) {
    position += model.CalculateCenterOffset(font);
  }
  
  auto &font_quad = get_font_quad(window_id);
  auto program = get_program(material);
  
  program->Bind();
  font_quad.Bind();
  
  //  Set face color.
  material->Configure(*program.get());
  program->SetUniform(eowu::uniforms::projection, projection_matrix);
  
  model.UseText([&](const auto &text) -> void {
    float x_offset = 0.0f;
    
    for (const auto &character : text) {
      single_char[0] = character;
      
      if (!font->HasGlyph(single_char)) {
        continue;
      }
      
      const auto &glyph = font->GetGlyph(single_char);
      
      position.x += x_offset;
      
      x_offset = font_quad.Configure(glyph, position, scale);
      
      glyph.Bind();
      program->SetUniform(eowu::uniforms::text, glyph.GetTexture());
      font_quad.Draw();
    }
  });
  
  font_quad.Unbind();
  program->Unbind();
}

void eowu::TextRenderer::Queue(const eowu::TextRenderer::DrawableComponent &component,
                               std::shared_ptr<eowu::Window> window) {
  const auto &id = window->GetIdentifier();
  
  require_models_container(id);
  
  queued_components.at(id).push_back(component);
}

void eowu::TextRenderer::ClearQueue() {
  for (auto &it : queued_components) {
    it.second.clear();
  }
}

void eowu::TextRenderer::NextFrame() {
  for (auto &it : queued_components) {
    auto &components = it.second;
    
    for (auto &component : components) {
      component.model.NextFrame();
    }
  }
}

void eowu::TextRenderer::RegisterWindows(const std::vector<std::shared_ptr<eowu::Window>> &windows) {
  for (const auto &win : windows) {
    register_window(win);
  }
}

void eowu::TextRenderer::RegisterWindow(std::shared_ptr<eowu::Window> window) {
  register_window(window);
}

void eowu::TextRenderer::register_window(const std::shared_ptr<eowu::Window> &window) {
  //  Create the font quad for this window, checking whether the font quad is
  //  already present.
  create_font_quad(window->GetIdentifier(), true);
}

const std::vector<eowu::TextRenderer::DrawableComponent>& eowu::TextRenderer::GetQueuedComponents(std::shared_ptr<eowu::Window> for_window) {
  
  const auto &id = for_window->GetIdentifier();
  
  require_models_container(id);
  
  return queued_components.at(id);
}

void eowu::TextRenderer::require_models_container(const eowu::Identifier &id) {
  auto it = queued_components.find(id);
  
  if (it == queued_components.end()) {
    queued_components.emplace(id, std::vector<eowu::TextRenderer::DrawableComponent>());
  }
}

eowu::FontQuad& eowu::TextRenderer::get_font_quad(const eowu::Identifier &for_window_id) {
  auto it = font_quads_by_window_id.find(for_window_id);
  
  if (it != font_quads_by_window_id.end()) {
    return it->second;
  }
  
  create_font_quad(for_window_id, true);
  
  return font_quads_by_window_id.at(for_window_id);
}

void eowu::TextRenderer::create_font_quad(const eowu::Identifier &for_window_id, bool check_if_exists) {
  if (check_if_exists && font_quads_by_window_id.count(for_window_id) > 0) {
    return;
  }
  
  eowu::FontQuad font_quad;
  font_quad.Create();
  
  font_quads_by_window_id[for_window_id] = font_quad;
}

std::shared_ptr<eowu::Program> eowu::TextRenderer::get_program(const std::shared_ptr<eowu::Material> &material) {
  auto identifier = material->GetIdentifier();
  auto by_material_it = programs_by_material_id.find(identifier);
  
  //  Program exists for this material id
  if (by_material_it != programs_by_material_id.end()) {
    return by_material_it->second;
  }
  
  //  Analyze material to see if we have a program
  //  for this source already
  const eowu::Material &mat = *material.get();
  
  auto vert_source = eowu::builder::vertex_text(mat);
  auto frag_source = eowu::builder::fragment_text(mat);
  
  auto full_source = vert_source + frag_source;
  
  auto by_source_it = programs_by_source.find(full_source);
  
  //  We have a program for this source already, so don't
  //  make a new one
  if (by_source_it != programs_by_source.end()) {
    auto prog = by_source_it->second;
    
    programs_by_material_id[identifier] = prog;
    
    return prog;
  }
  
  //  Otherwise, make the program
  auto prog = eowu::builder::from_source(vert_source, frag_source);
  
  programs_by_material_id[identifier] = prog;
  programs_by_source[full_source] = prog;
  
  return prog;
}
