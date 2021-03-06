//
//  Renderer.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "ContextManager.hpp"
#include "TextRenderer.hpp"
#include "Model.hpp"
#include "ProjectionTypes.hpp"
#include "Identifier.hpp"
#include "Program.hpp"
#include <eowu-common/Timer.hpp>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>

namespace eowu {
  class Renderer;
  
  using BufferSwapCallbackType = std::function<void(const eowu::WindowType&)>;
}

class eowu::Renderer {
  
public:
  Renderer();
  ~Renderer() = default;
  
  void Queue(const std::vector<eowu::Model> &models, eowu::WindowType window);
  void Queue(const eowu::Model &model, eowu::WindowType window);
  void Queue(const eowu::TextRenderer::DrawableComponent &component, eowu::WindowType window);
  void ClearQueue();
  
  void Draw();
  
  void SetClearColor(const glm::vec3 &color);
  void SetOnBufferSwap(const eowu::BufferSwapCallbackType &cb);
  eowu::time::DurationType Delta();
  
  void RegisterWindow(eowu::WindowType window);
  void RegisterWindows(const std::vector<eowu::WindowType> &windows);
private:
  mutable std::mutex mut;
  
  eowu::TextRenderer text_renderer;
  
  eowu::BufferSwapCallbackType on_buffer_swap;

  eowu::projection_types::Types projection_type;
  glm::vec3 clear_color;
  
  struct FrameTime {
    eowu::Timer timer;
  } frame_timing;
  
  std::unordered_map<eowu::Identifier, eowu::WindowType> windows;
  std::unordered_map<eowu::Identifier, std::vector<Model>> models;
  
  std::unordered_map<std::size_t, std::shared_ptr<eowu::Program>> programs;
  std::unordered_map<eowu::Identifier, std::size_t> programs_by_material_id;
  std::unordered_set<eowu::Identifier> analyzed_material_ids;
  
  std::shared_ptr<eowu::Program> last_program;
  std::shared_ptr<eowu::Mesh> last_mesh;
  eowu::WindowType last_window;
  
private:
  glm::mat4 get_projection_matrix(eowu::WindowType window) const;
  glm::mat4 get_view_matrix() const;
  
  std::vector<eowu::Model>& get_models_container(const eowu::WindowType &window);
  
  void configure_window_context(const eowu::WindowType &window);
  void next_frame();
  void draw(eowu::WindowType window);
  std::size_t draw_one_model(const eowu::Model &model,
                             const eowu::WindowType &window,
                             const glm::mat4 &projection_matrix,
                             const std::size_t is_first_model);
};
